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

# include "../includes/Server.hpp"

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
	// receiveClientMessage();
	socketMultiPlex();
}

void	Server::serverOff(void) {
	std::vector<User *>::iterator	iter;
	for (iter = _user_vector.begin(); iter != _user_vector.end(); iter++)
		delete (*iter);
	close(_server_socket);
	exit(0);
}

void	 Server::socketMultiPlex(void) {

	int	fd_max, str_len, fd_num;
	socklen_t adr_sz;
	int client_socket;
	struct sockaddr_in client_adr;

	fd_set	temp;

	FD_ZERO(&_reads);
	FD_SET(_server_socket, &_reads);
	fd_max = _server_socket;

	while (1) {
		temp = _reads;
		_timeout.tv_sec = 5;
		_timeout.tv_usec = 5000;

		if ((fd_num = select(fd_max + 1, &temp, 0, 0, &_timeout)) == -1)
			break;
		if (fd_num == 0)
			continue;
		
		for (int i = 0; i < fd_max + 1; i++) {
			if (FD_ISSET(i, &temp)) {
				if (i == _server_socket) {
					adr_sz = sizeof(client_adr);
					client_socket = accept(_server_socket, (struct sockaddr*)&client_adr, &adr_sz);
					FD_SET(client_socket, &_reads);
					if (fd_max < client_socket)
						fd_max = client_socket;
					std::cout << "connected client " << client_socket << " \n";
				}
				else {
					//str_len = read(i, _message, BUF_SIZE);
					//
					while ((str_len = recv(client_socket, _message, BUF_SIZE, 0)) != 0) {	// 데이터 송수신
						if (str_len == -1)
							continue;
						_message[str_len] = 0;
						// puts(msg);
						std::cout << _message << '\n';
						// write(cli_sock, msg, str_len);
					}
					//
					if (str_len == 0) {
						FD_CLR(i, &_reads);
						close(i);
						std::cout << "closed client : " << client_socket << "\n";
					} else {
						write(i, _message, str_len);
					}
					
				}
			}
		}
	}
	close(_server_socket);
}

void	Server::receiveClientMessage() {
	User	user;

	// while (1) {
		try {
			int	client_socket = accept(_server_socket, (struct sockaddr*)user.getAddressPtr(), user.getAddressSizePtr());
			if (client_socket < 0)
				throw Error::SocketOpenException();
			user.setSocketDesc(client_socket);
			std::string fullMsg = concatMessage(user.getSocketDesc());
			parseMessageStream(&user, fullMsg);
		} catch (std::exception &e) {
			std::cout << e.what() << "\n";
			close(user.getSocketDesc());
			//continue;
		}
		// sendToClientMessage(&user);
	//}
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
	ft_replaceStr(fullMsg, "\r", " ");

	return fullMsg;
}

void		Server::parseMessageStream(User* user, const std::string& fullMsg) {
	std::vector<std::string>			commands = ft_split(fullMsg, '\n');
	std::vector<std::string>::iterator	cmdIter;

	std::cout << fullMsg << "\n\n";

	for (cmdIter = commands.begin(); cmdIter != commands.end(); cmdIter++) {
		std::vector<std::string>	parameters = ft_split(*cmdIter, ' ');

		std::cout << *parameters.begin() << "\n";
		if (*parameters.begin() == CMD_NICK) commandNICK(user, parameters);
		else if (*parameters.begin() == CMD_USER) commandUser(user, parameters, _user_vector);
	}
}

void		Server::sendToClientMessage(User *user) {
	std::cerr << "here\n";

	char message[BUF_SIZE] = "welcome to SAN_JI_JICK_SONG";
	
	std::cerr << user->getSocketDesc() << "\n";
	if (send(user->getSocketDesc(), message, strlen(message), 0) == -1)
		std::cerr << "send() error\n";
	// write(user->getSocketDesc(), message, strlen(message));

}

