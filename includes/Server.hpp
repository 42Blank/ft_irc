#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <unistd.h>
# include <cstring>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <vector>

# include "User.hpp"

# define BUF_SIZE 1024

class Server {
private:
	int		serv_sock;
	char	msg[BUF_SIZE];
	int		str_len;

	struct sockaddr_in	serv_adr;

	std::vector<User *>	user_vector;

	void	socketCreate(void);
	void	errorHandling(std::string msg);


public:
	Server();
	Server(char *port);// 비번 추가 해야 함.
	~Server();

	void	serverOn(void);
	void	serverOff(void);

};

#endif
