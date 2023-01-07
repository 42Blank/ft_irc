/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2023/01/07 15:46:05 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"
# include <fcntl.h>

Server::Server(char* port, char* password) {
	struct pollfd	server_pollfd;

	_serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성
	if (_serverSocket < 0)
		throw std::runtime_error(Error(ERR_SERVEROPENFAILED, "socket"));

	_port = atoi(port);
	_password = password;
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	_serverAddress.sin_port = htons(_port);
	server_pollfd.fd = _serverSocket;
	server_pollfd.events = POLLIN;
	_poll_fds.push_back(server_pollfd);

	if (bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0)	// 소켓 주소 할당
		throw std::runtime_error(Error(ERR_SERVEROPENFAILED, "bind"));
}

Server::~Server(void) {
	serverOff();
}

void	Server::serverOn(void) {
	pollFdIter	iter;

	if (listen(_serverSocket, 5) < 0)
		throw std::runtime_error(Error(ERR_SERVEROPENFAILED, "listen"));

	while (true) {
		if (!poll(_poll_fds.data(), _poll_fds.size(), 1000)) continue ;
		if (_poll_fds[0].revents & POLLIN) {
			acceptClient();
			continue;
		}
		for (iter = _poll_fds.begin() + 1; iter < _poll_fds.end(); iter++) {
			if (iter->revents & POLLHUP) { // 현재 클라이언트 연결 끊김
				setUserDisconnectByFd(iter->fd);
				continue ;
			}
			else if (iter->revents & POLLIN) {
				if (isServerUser(iter->fd))
					receiveClientMessage(iter->fd);
				else
					receiveFirstClientMessage(iter->fd);
			}
			if (iter->revents & POLLNVAL) _poll_fds.erase(iter);
		}
		deleteChannel();
		disconnectClients();
	}
}

void	Server::serverOff(void) {
	close(_serverSocket);
	exit(0);
}

void	Server::acceptClient(void) {
	struct sockaddr_in	client_addr;
	int					clientSocket;
	struct pollfd		client_pollfd;
	size_t				client_len = sizeof(client_addr);

	try {
		clientSocket = accept(_serverSocket, (struct sockaddr*)&client_addr, (socklen_t*)&client_len);
		if (clientSocket < 0)
			throw std::runtime_error(Error(ERR_CLIENTCONNECTFAILED));

		fcntl(clientSocket, F_SETFL, O_NONBLOCK);
		client_pollfd.fd = clientSocket;
		client_pollfd.events = POLLIN;
		_poll_fds.push_back(client_pollfd);
	}
	catch (std::exception& e) {
		std::cout << e.what() << "\n";
		close(clientSocket);
	}
}

void	Server::receiveFirstClientMessage(int clientFd) {
	User*	user;

	try {
		user = new User();
		user->setSocketFd(clientFd);
		std::string	fullMsg = concatMessage(clientFd);
		parseMessageStream(user, fullMsg);
		_serverUser.push_back(user);
	}	catch (std::exception& e) {
		std::cout << e.what() << "\n";
		close(clientFd);
	}
}

void	Server::receiveClientMessage(int clientSocket) {
	try {
		std::string	fullMsg = concatMessage(clientSocket);
		User*		user = findUser(clientSocket);
		parseMessageStream(user, fullMsg);
	} catch (std::exception& e) {
		std::cout << e.what() << "\n";
	}
}

std::string	Server::concatMessage(int clientSocket) {
	int			message_length;
	std::string	fullMsg = "";

	while ((message_length = recv(clientSocket, _message, BUF_SIZE, 0)) != 0) {
		if (message_length < 0) continue;
		_message[message_length] = 0;
		fullMsg += _message;
		if (fullMsg.length() >= 2 && !fullMsg.substr(fullMsg.length() - 2).compare("\r\n")) break;
	}
	ft_replaceStr(fullMsg, "\r", " ");

	return fullMsg;
}

void	Server::parseMessageStream(User* user, const std::string& fullMsg) {
	stringVector	commands = ft_split(fullMsg, '\n');
	stringIter		cmdIter;

	std::cout << "======Message======\n" << fullMsg << "\n";

	for (cmdIter = commands.begin(); cmdIter != commands.end(); cmdIter++) {
		stringVector	parameters = ft_split(*cmdIter, ' ');

		if (!parameters[0].compare(CMD_CAP)) commandCAP(user, parameters);
		else if (!parameters[0].compare(CMD_PASS)) commandPASS(user, parameters);
		else if (!parameters[0].compare(CMD_NICK)) commandNICK(user, parameters);
		else if (!parameters[0].compare(CMD_USER)) commandUSER(user, parameters);
		else if (!parameters[0].compare(CMD_PING)) commandPING(user, parameters);
		else if (!parameters[0].compare(CMD_PONG)) commandPONG(user, parameters);
		else if (!parameters[0].compare(CMD_MODE)) commandMODE(user, parameters);
		else if (!parameters[0].compare(CMD_JOIN)) commandJOIN(user, parameters);
		else if (!parameters[0].compare(CMD_MSG)) commandMSG(user, parameters);
		else if (!parameters[0].compare(CMD_TOPIC)) commandTOPIC(user, parameters);
		else if (!parameters[0].compare(CMD_NAMES)) commandNAMES(user, parameters);
		else if (!parameters[0].compare(CMD_PART)) commandPART(user, parameters);
		else if (!parameters[0].compare(CMD_QUIT)) commandQUIT(user, parameters);
		else if (!parameters[0].compare(CMD_KICK)) commandKICK(user, parameters);
		else if (!parameters[0].compare(CMD_WHO)) commandWHO(user, parameters);
		else sendMessage(user, Error(ERR_UNKNOWNCOMMAND, parameters[0]));
	}
}

void	Server::deleteChannel() {

	channelIter	channelIter = _channelList.begin();

	while (channelIter < _channelList.end()){
		if ((*channelIter)->getIsDeleted()) {
			delete *channelIter;
			channelIter = _channelList.erase(channelIter);
		} else {
			channelIter++;
		}
	}	
}

void	Server::disconnectClients() {
	userIter	userIter = _serverUser.begin();
	pollFdIter	pollIter;

	while (userIter < _serverUser.end()) {
		if (!(*userIter)->getIsDisconnected()) {
			userIter++;
			continue;
		}
		close((*userIter)->getSocketFd());
		pollIter = _poll_fds.begin() + 1;
		while (pollIter < _poll_fds.end()) {
			if ((*userIter)->getSocketFd() == pollIter->fd) {
				_poll_fds.erase(pollIter);
				break ;
			}
			pollIter++;
		}
		delete *userIter;
		userIter = _serverUser.erase(userIter);
	}
}
