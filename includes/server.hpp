#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <unistd.h>
# include <cstring>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <vector>

# define BUF_SIZE 1024

class Server {
private:
	int		serv_sock;
	int		cli_sock; //User class로 이동
	char	msg[BUF_SIZE];
	int		str_len;
	
	struct sockaddr_in	serv_adr;
	struct sockaddr_in	cli_adr; //User class로 이동
	socklen_t	cli_adr_sz; //User class로 이동
	
	std::vector<int>	v;	// User class 담는 vector
	// std::vector<User>	v;	// User class 담는 vector

	void	socketCreate(void);
	void	errorHandling(std::string msg);


public:
	Server();
	Server(char *port);// 비번 추가 해야 함.
	~Server();

	void	serverOn(void);

};

#endif