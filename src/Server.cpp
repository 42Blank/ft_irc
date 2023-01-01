/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasong <jasong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2023/01/01 19:08:35 by jasong           ###   ########.fr       */
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
		if (_poll_fds[0].revents & POLLIN) { // _poll_fds[0] -> 서버 fd에 POLLIN event발생
			welcomeProcess();
			continue;
		}
		std::vector<struct pollfd>::iterator iter;

		for (iter = _poll_fds.begin() + 1; iter < _poll_fds.end(); iter++) {
			if (iter->revents & POLLHUP) { // 현재 클라이언트 연결 끊김
				close((*iter).fd);
				try  {
					int	idx = getUserIndexByFd((*iter).fd);
					_user_vector.erase(_user_vector.begin() + idx);
				}
				catch (std::exception &e) {
					std::cout << e.what() << "\n";
				}
				_poll_fds.erase(iter);
			}
			else if (iter->revents & POLLIN) {
				receiveClientMessage(iter->fd);
				ft_checkPollReturnEvent(iter->revents);
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

void	Server::welcomeProcess(void) {
	struct pollfd client_pollfd;
	User user;

	try {
		int	clientSocket = accept(_serverSocket, (struct sockaddr*)user.getAddressPtr(), user.getAddressSizePtr());
		if (clientSocket < 0)
			throw Error::SocketOpenException();
		user.setSocketDesc(clientSocket);
		std::string fullMsg = concatMessage(user.getSocketDesc());
		parseWelcomeMessageStream(user, fullMsg);
		this->testUser(); // 테스트용 함수
		client_pollfd.fd = user.getSocketDesc();
		client_pollfd.events = POLLIN;
		_poll_fds.push_back(client_pollfd);
	}	catch (std::exception &e) {
		std::cout << e.what() << "\n";
		close(user.getSocketDesc()); // 웰컴 프로세스 실패 시에는 유저 통신 끊어야함
	}
}

void	Server::receiveClientMessage(int clientSocket) {
	try {
		std::string	fullMsg = concatMessage(clientSocket);
		int			userIdx = getUserIndexByFd(clientSocket);
		parseMessageStream(_user_vector[userIdx], fullMsg);
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

void	Server::parseWelcomeMessageStream(User &user, const std::string& fullMsg) {
	std::vector<std::string>			commands = ft_split(fullMsg, '\n');
	std::vector<std::string>::iterator	cmdIter;

	std::cout << "\n======Message======\n" << fullMsg << "\n";

	for (cmdIter = commands.begin(); cmdIter != commands.end(); cmdIter++) {
		std::vector<std::string>	parameters = ft_split(*cmdIter, ' ');

		// std::cout << "parameter : " << *cmdIter << "\n";
		if (*parameters.begin() == CMD_NICK) commandNICK(user, parameters);
		else if (*parameters.begin() == CMD_USER) commandUser(user, parameters);
		else continue;
	}
}

void	Server::parseMessageStream(User &user, const std::string& fullMsg) {
	std::vector<std::string>			commands = ft_split(fullMsg, '\n');
	std::vector<std::string>::iterator	cmdIter;

	std::cout << "\n======Message======\n" << fullMsg << "\n";

	for (cmdIter = commands.begin(); cmdIter != commands.end(); cmdIter++) {
		std::vector<std::string>	parameters = ft_split(*cmdIter, ' ');
		// if (*parameters.begin() == CMD_JOIN) commandJoin(user, parameters);
	}
	(void)user; // unused parameter때문에 꼼수씀
}

int	Server::getUserIndexByFd(int fd) {
	std::vector<User>::iterator	iter;
	int							index;

	index = 0;
	for (iter = _user_vector.begin(); iter < _user_vector.end(); iter++) {
		if (fd == (*iter).getSocketDesc())
			return (index);
		index++;
	}
	throw Error::UserNotFoundException();
	return (-1);
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

void		Server::testUser(void) {
	std::vector<User>::iterator	iter;

	for (iter = _user_vector.begin(); iter < _user_vector.end(); iter++) {
		std::cout << (*iter) << '\n';
	}
}
