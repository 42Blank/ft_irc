/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2022/12/31 17:25:54 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server_multi.hpp"
#include <poll.h>
#include <fcntl.h>

# define MAX_POLL 100

Server::Server(char *port) {

	int enable = 1;
	_serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성
	if (_serverSocket < 0)
		throw Error::SocketOpenException();
	setPort(atoi(port));
	memset(&_serverAddress, 0, sizeof(_serverAddress));	// 구조체 변수의 모든 멤버를 0으로 초기화
	_serverAddress.sin_family = AF_INET;			// 주소 체계 지정, AF_INET : IPv4 인터넷 프로토콜에 적용
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소
	_serverAddress.sin_port = htons(_port);	// port 번호 초기화
	_created_time = time(NULL);

	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR , &enable, sizeof(enable)))
		throw Error::SocketOpenException();
	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) < 0)
		throw Error::SocketOpenException();
	if (bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0)	// 소켓 주소 할당
		throw Error::SocketOpenException();
}

Server::~Server(void) {
	std::cout << "destructor called\n";
	serverOff();
}

void	Server::serverOn(void) {
	if (listen(_serverSocket, 5) < 0)	// 연결요청 대기상태
		throw Error::SocketOpenException();

	_fdList.push_back(pollfd());
	_fdList[0].fd = _serverSocket;
	_fdList[0].events = POLLIN;
	
	while (1)
		this->receiveClientMessage();
}

void	Server::serverOff(void) {
	close(_serverSocket);
	exit(0);
}

void	Server::sendClientMessage(User* user, std::string str) {
	std::string strToSend = str + "\r\n";
	if (send(user->getSocketDesc(), (strToSend).c_str(), strToSend.length(), 0) == -1)
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
		parseMessageStream(&user, fullMsg);
		this->testUser();
	} catch (std::exception &e) {
		std::cout << e.what() << "\n";
		close(user.getSocketDesc());
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

void	Server::parseMessageStream(User* user, const std::string& fullMsg) {
	std::vector<std::string>			commands = ft_split(fullMsg, '\n');
	std::vector<std::string>::iterator	cmdIter;

	std::cout << "\n======Message======\n" << fullMsg << "\n";

	for (cmdIter = commands.begin(); cmdIter != commands.end(); cmdIter++) {
		std::vector<std::string>	parameters = ft_split(*cmdIter, ' ');

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
