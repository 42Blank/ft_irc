#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void	error_handling (char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main (int ac, char **av) {

	int		serv_sock, cli_sock;
	char	msg[BUF_SIZE];
	int 	str_len, i;

	struct sockaddr_in	serv_adr;
	struct sockaddr_in	cli_adr;
	socklen_t cli_adr_sz;

	if (ac != 2)
	{
		printf("Usage : %s <port>\n", av[0]);
		exit(1);	
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(av[1]));

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	cli_adr_sz = sizeof(cli_adr);

	for (i = 0; i < 5; i++) {
		cli_sock = accept(serv_sock, (struct sockaddr*)&cli_adr, &cli_adr_sz);
		if (cli_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);
		while ((str_len = recv(cli_sock, msg, BUF_SIZE, 0)) != 0) {
			if (str_len == -1)
				continue;
			msg[str_len] = 0;
			puts(msg);
			// write(cli_sock, msg, str_len);
		}
		close(cli_sock);
	}
	close(serv_sock);
	return 0;
}