/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2022/12/31 17:26:36 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "User.hpp"
# include "Error.hpp"

# define BUF_SIZE 1024
# define SERVER_NAME "San-Ji-jik-Song_IRCServ"

# define CMD_CAP "CAP"
# define CMD_PASS "PASS"
# define CMD_NICK "NICK"
# define CMD_USER "USER"

class Server {
	private:
		int					_port;
		std::string			_password;
		int					_serverSocket;
		struct sockaddr_in	_serverAddress;
		std::vector<User>	_user_vector;
		time_t				_created_time;
		char				_message[BUF_SIZE];
		struct timeval		_timeout;
		fd_set				_reads;

		void		sendClientMessage(User* user, std::string str);
		void		receiveClientMessage();
		std::string	concatMessage(int clientSocket);
		void		parseMessageStream(User* user, const std::string& fullMsg);

		void		commandCAP(User* user, std::vector<std::string>& parameters);
		void		commandPASS(User* user, std::vector<std::string>& parameters);
		void		commandNICK(User* user, std::vector<std::string>& parameters);
		void		commandUser(User* user, std::vector<std::string>& parameters);

	public:
		Server(char *port);// 비번 추가 해야 함.
		~Server();

		void		serverOn(void);
		void		serverOff(void);
		void		testUser(void);

		int			getPort() const;
		void		setPort(int port);
		std::string	getPassword() const;
		void		setPassword(std::string password);
};

#endif
