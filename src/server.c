#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void	error_handling (char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main (int ac, char **av) {

	int		serv_sock;
	/* serv_sock -> 문지기 
		클라이언트의 연결요청을 대기실로 안내하거나(연결요청 대기 큐) 연결한다. 
	*/
	int		cli_sock;
	/* cli_sock -> 클라이언트와 데이터를 주고 받는 소켓
		serv_sock은 연결요청과 그 대기실을 관리해야 하기 때문에 클라이언트와 데이터를 주고 받을 소켓을 하나 더 만들어야 한다. 
		이 소켓은 직접 socket으로 할당할 필요는 없고 accept 함수의 결과로 할당되게 된다. 
	*/
	char	msg[BUF_SIZE];
	int 	str_len, i;

	struct sockaddr_in	serv_adr; // 인터넷 주소 정보를 저장할 변수
	struct sockaddr_in	cli_adr;
	socklen_t cli_adr_sz;

	if (ac != 2)
	{
		printf("Usage : %s <port>\n", av[0]);
		exit(1);	
	}

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
	/*
		serv_sock : 연결 요청 대기 상태에 두고자 하는 소켓의 패일 디스크립터 전달, 
					이 디스크립터의 소켓이 서버 소켓(리스닝 소켓)이 된다. 
		5 : 연결요청 대기 큐의 크기를 지정하는 파라미터, 클라이언트의 연결 요청을 5개까지 대기시킬 수 있다. 
	*/
		error_handling("listen() error");
	
	cli_adr_sz = sizeof(cli_adr);

	for (i = 0; i < 5; i++) {
		cli_sock = accept(serv_sock, (struct sockaddr*)&cli_adr, &cli_adr_sz);	// 연결 허용
		/*
			serv_sock : 서버(문지기) 소켓의 파일 디스크립터
			&cli_adr : 연결 요청한 클라이언트의 주소 정보를 담을 수 있는 변수의 주소 값
						이 변수에 클라이언트의 주소 정보가 채워진다. 
			&cli_adr_sz : 두번째 매개변수의 크기를 바이트 단위로 전달한다. 
		*/
		if (cli_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);
		while ((str_len = recv(cli_sock, msg, BUF_SIZE, 0)) != 0) {	// 데이터 송수신
			if (str_len == -1)
				continue;
			msg[str_len] = 0;
			puts(msg);
			// write(cli_sock, msg, str_len);
		}
		close(cli_sock);
	}
	close(serv_sock);	// 연결 종료
	return 0;
}