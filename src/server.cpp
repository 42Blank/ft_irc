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

	close(this->serv_sock);
}

void	Server::serverOn(void) {

	if (listen(this->serv_sock, 5) == -1)	// 연결요청 대기상태 
		errorHandling("listen() error");

	this->cli_adr_sz = sizeof(this->cli_adr);

	for (int i = 0; i < 5; i++) {
		this->cli_sock = accept(this->serv_sock, (struct sockaddr*)&this->cli_adr, &this->cli_adr_sz);	// 연결 허용
		if (this->cli_sock == -1)
			errorHandling("accept() error");
		else
			std::cout << "Connected client " << i + 1 << '\n';
		while ((this->str_len = recv(this->cli_sock, this->msg, BUF_SIZE, 0)) != 0) {	// 데이터 송수신
			if (this->str_len == -1)
				continue;
			this->msg[this->str_len] = 0;
			// puts(this->msg);
			std::cout << this->msg << '\n';
			// write(cli_sock, msg, str_len);
		}
		close(this->cli_sock);
	}
}

void	Server::errorHandling(std::string msg) {

	std::cout << msg << '\n';
	close(this->serv_sock);
	exit(1);
}