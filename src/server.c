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
	/* serv_sock -> 문지기
		클라이언트의 연결요청을 대기실로 안내하거나(연결요청 대기 큐) 연결한다.
	*/
	int		cli_sock;
	/* cli_sock -> 클라이언트와 데이터를 주고 받는 소켓
		serv_sock은 연결요청과 그 대기실을 관리해야 하기 때문에 클라이언트와 데이터를 주고 받을 소켓을 하나 더 만들어야 한다.
		이 소켓은 직접 socket으로 할당할 필요는 없고 accept 함수의 결과로 할당되게 된다.
	*/
	char	msg[BUF_SIZE];

	// int 	str_len, i;//

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
	/*
		serv_sock : 연결 요청 대기 상태에 두고자 하는 소켓의 패일 디스크립터 전달,
					이 디스크립터의 소켓이 서버 소켓(리스닝 소켓)이 된다.
		5 : 연결요청 대기 큐의 크기를 지정하는 파라미터, 클라이언트의 연결 요청을 5개까지 대기시킬 수 있다.
	*/
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
