/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2022/12/30 03:19:25 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Error.hpp"

Server::Server() {}

Server::Server(char *port) {
	_server_socket = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성
	if (_server_socket < 0)
		throw Error::SocketOpenException();

	memset(&_server_address, 0, sizeof(_server_address));	// 구조체 변수의 모든 멤버를 0으로 초기화
	_server_address.sin_family = AF_INET;			// 주소 체계 지정, AF_INET : IPv4 인터넷 프로토콜에 적용
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소
	_server_address.sin_port = htons(atoi(port));	// port 번호 초기화

	if (bind(_server_socket, (struct sockaddr *)&_server_address, sizeof(_server_address)) < 0)	// 소켓 주소 할당
		throw Error::SocketOpenException();
}

Server::~Server() {
	std::cout << "destructor called\n";
	serverOff();
}

void	Server::serverOn(void) {
	if (listen(_server_socket, 5) < 0)	// 연결요청 대기상태
		throw Error::SocketOpenException();
	connectWithClient();
}

void	Server::serverOff(void) {
	std::vector<User *>::iterator	iter;
	for (iter = _user_vector.begin(); iter != _user_vector.end(); iter++)
		delete (*iter);
	close(_server_socket);
}

void	Server::connectWithClient() {
	int			client_socket;

	while (1) {
		try {
			User* user = new User();
			client_socket = accept(_server_socket, (struct sockaddr*)user->getAddressPtr(), user->getAddressSizePtr());
			if (client_socket < 0) {
				delete user;
				throw Error::SocketOpenException();
			}
			user->setSocketDesc(client_socket);
			// std::cout << "Client connected: " << i + 1 << "\n";

			std::string fullMsg = concatMessage(user->getSocketDesc());
			parseUserData(user, fullMsg);

		_user_vector.push_back(user); // 유저 검증 성공했을 때만 push back (실패하면 메모리 해제해야함)
		std::cout << "Message: " << fullMsg << "\n";
		// close(user->getSocketDesc());
		} catch (std::exception &e) {
			std::cout << e.what() << "\n";
			continue;
		}
	}
}

std::string	Server::concatMessage(int client_socket) {
	int		message_length;
	std::string	fullMsg = "";

	while ((message_length = recv(client_socket, _message, BUF_SIZE, 0)) != 0) {
		if (message_length < 0) continue;
		_message[message_length] = 0;
		fullMsg += _message;
		if (fullMsg.length() >= 2 && fullMsg.substr(fullMsg.length() - 2) == "\r\n") break;
	}
	return fullMsg;
}

void		Server::parseUserData(User* user, const std::string& fullMsg) {
	size_t pos = fullMsg.find("NICK");
	std::cout << "NICK position: [" << pos << "]\n";
	std::cout << "NICK: [" << fullMsg.at(pos) << "\\n";
	std::cout << "User [" << user << "]\n";
}
