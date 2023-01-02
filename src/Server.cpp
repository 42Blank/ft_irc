/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2023/01/03 01:31:27 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

Server::Server(char* port, char* password) {
	struct pollfd server_pollfd;

	_serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성
	if (_serverSocket < 0)
		throw std::runtime_error(Error(ERR_SERVEROPENFAILED, "socket"));

	_port = atoi(port);
	_password = password;
	memset(&_serverAddress, 0, sizeof(_serverAddress));	// 구조체 변수의 모든 멤버를 0으로 초기화
	_serverAddress.sin_family = AF_INET;			// 주소 체계 지정, AF_INET : IPv4 인터넷 프로토콜에 적용
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소
	_serverAddress.sin_port = htons(_port);	// port 번호 초기화
	_created_time = time(NULL);
	server_pollfd.fd = _serverSocket;
	server_pollfd.events = POLLIN;
	_poll_fds.push_back(server_pollfd);

	if (bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0)	// 소켓 주소 할당
		throw std::runtime_error(Error(ERR_SERVEROPENFAILED, "bind"));
}

Server::~Server(void) {
	std::cout << "destructor called\n";
	serverOff();
}

void	Server::serverOn(void) {
	std::vector<struct pollfd>::iterator iter;

	if (listen(_serverSocket, 5) < 0)	// 연결요청 대기상태
		throw std::runtime_error(Error(ERR_SERVEROPENFAILED, "listen"));
	while (true) {
		if (poll(_poll_fds.data(), _poll_fds.size(), 1000) == 0)
			continue ;
		if (_poll_fds[0].revents & POLLIN) { // _poll_fds[0] -> 서버 fd에 POLLIN event발생
			receiveFirstClientMessage();
			continue;
		}
		for (iter = _poll_fds.begin() + 1; iter < _poll_fds.end(); iter++) {
			if (iter->revents & POLLHUP) // 현재 클라이언트 연결 끊김
				removeClient(iter);
			else if (iter->revents & POLLIN)
				receiveClientMessage(iter->fd);
			ft_checkPollReturnEvent(iter->revents);
		}
	}
}

void	Server::serverOff(void) {
	close(_serverSocket);
	exit(0);
}

void	Server::sendClientMessage(User& user, std::string str) {
	std::string strToSend = ":" + std::string(SERVER_NAME) + " " + str;
	std::cout << strToSend << "\n";
	if (send(user.getSocketDesc(), (strToSend  + "\r\n").c_str(), strToSend.length(), 0) == -1)
		throw std::runtime_error(Error(ERR_MESSAGESENDFAILED));
}

void	Server::receiveFirstClientMessage(void) {
	struct pollfd client_pollfd;
	User user;

	try {
		int	clientSocket = accept(_serverSocket, (struct sockaddr*)user.getAddressPtr(), user.getAddressSizePtr());
		if (clientSocket < 0)
			throw std::runtime_error(Error(ERR_CLIENTCONNECTFAILED));
		user.setSocketDesc(clientSocket);
		std::string fullMsg = concatMessage(user.getSocketDesc());
		parseMessageStream(user, fullMsg);
		client_pollfd.fd = clientSocket;
		client_pollfd.events = POLLIN;
		_poll_fds.push_back(client_pollfd);
		_userList.push_back(user);
	}	catch (std::exception &e) {
		std::cout << e.what() << "\n";
		close(user.getSocketDesc()); // 위의 절차 중 하나라도 실패 시에는 유저 통신 끊어야함
	}
}

void	Server::receiveClientMessage(int clientSocket) {
	try {
		std::string	fullMsg = concatMessage(clientSocket);
		int			userIdx = getUserIndexByFd(clientSocket);
		parseMessageStream(_userList[userIdx], fullMsg);
	} catch (std::exception &e) {
		std::cout << e.what() << "\n";
	}
}

std::string	Server::concatMessage(int clientSocket) {
	int		message_length;
	std::string	fullMsg = "";

	while ((message_length = recv(clientSocket, _message, BUF_SIZE, 0)) != 0) {
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
	bool								isAllCommandInvalid = false;

	std::cout << "\n======Message======\n" << fullMsg << "\n";

	for (cmdIter = commands.begin(); cmdIter != commands.end(); cmdIter++) {
		std::vector<std::string>	parameters = ft_split(*cmdIter, ' ');

		isAllCommandInvalid = ft_checkIsCommandValid(parameters[0], user.getIsVerified());
		if (parameters[0] == CMD_PASS) commandPASS(user, parameters);
		else if (parameters[0] == CMD_NICK) commandNICK(user, parameters);
		else if (parameters[0] == CMD_USER) commandUSER(user, parameters);
		else if (user.getIsVerified() == ALL_VERIFIED) {
			if (parameters[0] == CMD_JOIN) commandJOIN(user, parameters);
			else if (parameters[0] == CMD_MSG) commandMSG(user, parameters);
			else if (parameters[0] == CMD_TOPIC) commandTOPIC(user, parameters);
			else if (parameters[0] == CMD_NAMES) commandNAMES(user, parameters);
			else if (parameters[0] == CMD_PART) commandPART(user, parameters);
		}
		else sendClientMessage(user, parameters[0] + " " + ERR_UNKNOWNCOMMAND);
	}
	if (!isAllCommandInvalid) throw std::runtime_error(Error(ERR_NOTREGISTERED));
}

void	Server::removeClient(std::vector<struct pollfd>::iterator fdIter) {
	try  {
		close((*fdIter).fd);
		int	idx = getUserIndexByFd((*fdIter).fd);
		_userList.erase(_userList.begin() + idx);
		_poll_fds.erase(fdIter);
	}
	catch (std::exception &e) {
		std::cout << e.what() << "\n";
	}
}

int	Server::getUserIndexByFd(int fd) {
	std::vector<User>::iterator	iter;
	int							index;

	index = 0;
	for (iter = _userList.begin(); iter < _userList.end(); iter++) {
		if (fd == (*iter).getSocketDesc())
			return (index);
		index++;
	}
	throw std::runtime_error(Error(ERR_CANNOTFINDUSERFD));
	return (-1);
}

////////////////////////////////////////// FOR DEBUG

void		Server::testUser(void) {
	std::vector<User>::iterator	iter;

	for (iter = _userList.begin(); iter < _userList.end(); iter++) {
		std::cout << (*iter) << '\n';
	}
}
