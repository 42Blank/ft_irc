/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasong <jasong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2022/12/31 21:18:23 by jasong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

Server::Server(char *port) {
	struct pollfd server_pollfd;

	_serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성
	if (_serverSocket < 0)
		throw Error::SocketOpenException();

	setPort(atoi(port));
	memset(&_serverAddress, 0, sizeof(_serverAddress));	// 구조체 변수의 모든 멤버를 0으로 초기화
	_serverAddress.sin_family = AF_INET;			// 주소 체계 지정, AF_INET : IPv4 인터넷 프로토콜에 적용
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소
	_serverAddress.sin_port = htons(_port);	// port 번호 초기화
	_created_time = time(NULL);
	server_pollfd.fd = _serverSocket;
	server_pollfd.events = POLLIN;
	_poll_fds.push_back(server_pollfd);

	if (bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0)	// 소켓 주소 할당
		throw Error::SocketOpenException();
}

Server::~Server(void) {
	std::cout << "destructor called\n";
	serverOff();
}

void	Server::serverOn(void) {
	int	poll_result;

	if (listen(_serverSocket, 5) < 0)	// 연결요청 대기상태
		throw Error::SocketOpenException();
	while (true) {
		poll_result = poll(_poll_fds.data(), _poll_fds.size(), 1000);
		if (poll_result == 0) {
			// std::cout << "Poll timeout\n";
			continue ;
		}
		if (_poll_fds[0].revents & POLLIN) {
			struct pollfd client_pollfd;

			receiveClientMessage(); // 새 클라이언트 추가
			if (_user_vector.empty())
				std::cout << "user vector is empty" << std::endl;
			client_pollfd.fd = _user_vector[_user_vector.size() - 1].getSocketDesc();
			client_pollfd.events = POLLIN;
			_poll_fds.push_back(client_pollfd);
		} else {
			std::vector<struct pollfd>::iterator iter;
			for (iter = _poll_fds.begin() + 1; iter < _poll_fds.end(); iter++) {
				if (iter->revents & POLLIN) {
					std::string fullStr = concatMessage(iter->fd);
					std::cout << fullStr << std::endl;
				}
			}
		}
	}
}

void	Server::serverOff(void) {
	close(_serverSocket);
	exit(0);
}

void	Server::sendClientMessage(User& user, std::string str) {
	std::string strToSend = str + "\r\n";
	if (send(user.getSocketDesc(), (strToSend).c_str(), strToSend.length(), 0) == -1)
		throw Error::SendMessageException();
}

void	Server::receiveClientMessage(void) {
	User user;

	try {
		int	clientSocket = accept(_serverSocket, (struct sockaddr*)user.getAddressPtr(), user.getAddressSizePtr());
		if (clientSocket < 0)
			throw Error::SocketOpenException();
		user.setSocketDesc(clientSocket);
		std::string fullMsg = concatMessage(user.getSocketDesc());
		parseMessageStream(user, fullMsg);
		this->testUser();
	} catch (std::exception &e) {
		std::cout << e.what() << "\n";
		close(user.getSocketDesc());
	}
}

std::string	Server::concatMessage(int clientSocket) {
	int		message_length;
	std::string	fullMsg = "";
	// int		i = 0;

	while ((message_length = recv(clientSocket, _message, BUF_SIZE, 0)) != 0) {
		// std::cout << i++ << " : " << _message << std::endl;
		if (message_length < 0) continue;
		_message[message_length] = 0;
		fullMsg += _message;
		if (fullMsg.length() >= 2 && fullMsg.substr(fullMsg.length() - 2) == "\r\n") break;
	}
	ft_replaceStr(fullMsg, "\r", " ");

	return fullMsg;
}

void	Server::parseMessageStream(User &user, const std::string& fullMsg) {
	std::vector<std::string>			commands = ft_split(fullMsg, '\n');
	std::vector<std::string>::iterator	cmdIter;

	std::cout << "\n======Message======\n" << fullMsg << "\n";

	for (cmdIter = commands.begin(); cmdIter != commands.end(); cmdIter++) {
		std::vector<std::string>	parameters = ft_split(*cmdIter, ' ');

		// std::cout << "parameter : " << *cmdIter << "\n";
		if (*parameters.begin() == CMD_CAP) commandCAP(user, parameters);
		else if (*parameters.begin() == CMD_NICK) commandNICK(user, parameters);
		else if (*parameters.begin() == CMD_USER) commandUser(user, parameters);
	}
}

int	Server::getPort() const {
	return _port;
}

void	Server::setPort(int port) {
	_port = port;
}

std::string	Server::getPassword() const {
	return _password;
}

void	Server::setPassword(std::string password) {
	_password = password;
}
