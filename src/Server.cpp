#include "../includes/Server.hpp"

Server::Server() {}

Server::Server(char *port) {
	this->serv_sock = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성
	if (this->serv_sock == -1)
		errorHandling("socket() error");

	memset(&this->serv_adr, 0, sizeof(this->serv_adr));	// 구조체 변수의 모든 멤버를 0으로 초기화
	this->serv_adr.sin_family = AF_INET;			// 주소 체계 지정, AF_INET : IPv4 인터넷 프로토콜에 적용
	this->serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);	// IP 주소
	this->serv_adr.sin_port = htons(atoi(port));	// port 번호 초기화

	if (bind(serv_sock, (struct sockaddr *)&this->serv_adr, sizeof(this->serv_adr)) == -1)	// 소켓 주소 할당
		errorHandling("bind() error");
}

Server::~Server() {
	std::cout << "destructor called\n";
	this->serverOff();
}

void	Server::serverOn(void) {
	int client_socket;
	std::string fullMsg;

	if (listen(this->serv_sock, 5) == -1)	// 연결요청 대기상태
		errorHandling("listen() error");

	for (int i = 0; i < 5; i++) {
		fullMsg = "";
		User* user = new User();
		client_socket = accept(this->serv_sock, (struct sockaddr*)user->getAddressPtr(), user->getAddressSizePtr());
		if (client_socket < 0) {
			delete user;
			errorHandling("accept() error");
		}
		user->setSocketDesc(client_socket);
		std::cout << "Client connected: " << i + 1 << "\n";
		while ((this->str_len = recv(user->getSocketDesc(), this->msg, BUF_SIZE, 0)) != 0) {
			if (this->str_len == -1) continue;
			this->msg[this->str_len] = 0;
			fullMsg += this->msg;
			if (fullMsg.substr(fullMsg.length() - 2) == "\r\n") break;
		}

		fullMsg.find("NICK");

		this->user_vector.push_back(user); // 유저 검증 성공했을 때만 push back (실패하면 메모리 해제해야함)
		std::cout << "Message: " << fullMsg << "\n";
		// close(user->getSocketDesc());
	}
}

void	Server::errorHandling(std::string msg) {

	std::cout << "ircserv: " << msg << '\n';
	serverOff();
	exit(1);
}

void	Server::serverOff(void) {
	std::vector<User *>::iterator	iter;
	for (iter = this->user_vector.begin(); iter != this->user_vector.end(); iter++)
		delete (*iter);
	close(this->serv_sock);
}

