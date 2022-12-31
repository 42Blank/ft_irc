#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>

#define BUF_SIZE 1024
#define MAX_POLL 100

void	error_handling (char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main (int ac, char **av) {

	int		serv_sock;
	int		cli_sock;
	char	msg[BUF_SIZE];
	struct sockaddr_in	serv_adr; // 인터넷 주소 정보를 저장할 변수
	struct sockaddr_in	cli_adr;
	socklen_t cli_adr_sz;

	int fd_count = 0; //

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성
	if (serv_sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));	// 구조체 변수의 모든 멤버를 0으로 초기화
	serv_adr.sin_family = AF_INET;			// 주소 체계 지정, AF_INET : IPv4 인터넷 프로토콜에 적용
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소
	serv_adr.sin_port = htons(atoi(av[1]));	// port 번호 초기화

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)	// 소켓 주소 할당
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)	// 연결요청 대기상태
		error_handling("listen() error");

	// pollfd 배열 구조체 생성
	struct pollfd fd_list[MAX_POLL];

	// server socket에대한 이벤트 등록
	fd_list[0].fd = serv_sock; // 0번째 배열에는 listen을 지정
	fd_list[0].events = POLLIN; // 읽도록 만든다.
	fd_list[0].revents = 0; // 처음에는 0으로 초기화 한다(아직 아무 일도 일어나지 않았으니)
	fd_count++;

	int i;
	for (i = 1; i < MAX_POLL; i++)
		fd_list[i].fd = -1;

	cli_adr_sz = sizeof(cli_adr);


	while (1) {
		int result = poll(fd_list, fd_count, -1);

		if (result > 0) {
			if (fd_list[0].revents == POLLIN) {
				
				cli_sock = accept(serv_sock, (struct sockaddr *) &cli_adr, &cli_adr_sz);
				printf("클라이언트가 접속됨:\n");
				printf("IP: %s PORT: %d\n", inet_ntoa(cli_adr.sin_addr), ntohs(cli_adr.sin_port));
				fd_list[fd_count].fd = cli_sock;
				fd_list[fd_count].events = POLLIN;
				fd_count++;
			} else {
				int str_len;
				for (i = 1; i < fd_count; i++) {
					switch (fd_list[i].revents) {
						case 0:
							break;
						case POLLIN: // echo
							str_len = read(fd_list[i].fd, msg, BUF_SIZE);
							printf("%d bytes read\n", str_len);

							msg[str_len] = 0;
							fputs(msg, stdout);
							fflush(stdout);
							// client 로 echo 응답
							write(fd_list[i].fd, msg, strlen(msg));
						default: // 슬롯 초기화
							close(fd_list[i].fd);
							fd_list[i].fd = -1;
							fd_list[i].revents = 0;
					}
				}
			}
		} else {
			error_handling("failed poll()");
			return -1;
		}
	}
	close (serv_sock);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////


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

