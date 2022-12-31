/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2022/12/30 05:10:11 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server_multi.hpp"
#include <poll.h>

# define MAX_POLL 100

Server::Server(char *port) {
	_serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성
	if (_serverSocket < 0)
		throw Error::SocketOpenException();

	setPort(atoi(port));
	memset(&_serverAddress, 0, sizeof(_serverAddress));	// 구조체 변수의 모든 멤버를 0으로 초기화
	_serverAddress.sin_family = AF_INET;			// 주소 체계 지정, AF_INET : IPv4 인터넷 프로토콜에 적용
	_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소
	_serverAddress.sin_port = htons(_port);	// port 번호 초기화
	_created_time = time(NULL);

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
	
	int fd_count = 0;
	struct pollfd	fd_list[MAX_POLL];
	
	// server socket에대한 이벤트 등록
	fd_list[0].fd = _serverSocket; // 0번째 배열에는 listen을 지정
	fd_list[0].events = POLLIN; // 읽도록 만든다.
	fd_list[0].revents = 0; // 처음에는 0으로 초기화 한다(아직 아무 일도 일어나지 않았으니)
	fd_count++;

	int i;
	for (i = 1; i < MAX_POLL; i++)
		fd_list[i].fd = -1;

	while (1) {
		int result = poll(fd_list, fd_count, -1);
		this->receiveClientMessage(fd_list, result, fd_count);
	}
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

void	Server::receiveClientMessage(struct pollfd *fd_list, int result, int fd_count) {
	User user;

	if (result > 0) {
		if (fd_list[0].revents == POLLIN) {
			int clientSocket = accept(_serverSocket, (struct sockaddr *)user.getAddressPtr(), user.getAddressSizePtr());
			if (clientSocket < 0)
				exit(1);
			printf("클라이언트가 접속됨:\n");
			user.setSocketDesc(clientSocket);
			for (int i = 1; i < MAX_POLL; i++)
			{
				if (fd_list[i].fd == -1) // 비어있는 fd슬롯을 찾아서 넣어준다.
				{
					fd_list[i].fd = clientSocket;
					fd_list[i].events = POLLIN;
					fd_list[i].revents = 0;
					break; // 모두 다 넣고 break를 통해 한번만 실행
				}
			}
			// printf("IP: %s PORT: %d\n", inet_ntoa(cli_adr.sin_addr), ntohs(cli_adr.sin_port));
			// fd_list[fd_count].fd = clientSocket;
			// fd_list[fd_count].events = POLLIN;
			// fd_count++;
		} 
		int str_len = 0;
		for (int i = 1; i < fd_count; i++) {
			switch (fd_list[i].revents) {
				case 0:
					break;
				case POLLIN: // echo
					// std::string fullMsg = concatMessage(user.getSocketDesc());
					parseMessageStream(&user, concatMessage(user.getSocketDesc()));
					this->testUser();
					printf("%d bytes read\n", str_len);
				default: // 슬롯 초기화
					close(fd_list[i].fd);
					fd_list[i].fd = -1;
					fd_list[i].revents = 0;
			}
		}
	} else {
		std::cerr << "failed poll()";
		exit(1);
	}
	close (_serverSocket);
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
