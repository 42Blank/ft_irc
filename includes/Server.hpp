/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2023/01/02 21:27:04 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "User.hpp"
# include "Error.hpp"
# include "Channel.hpp"

# define BUF_SIZE 1024
# define SERVER_NAME "San-Ji-jik-Song_IRCServ"

# define CMD_PASS "PASS"
# define CMD_NICK "NICK"
# define CMD_USER "USER"
# define CMD_JOIN "JOIN"
# define CMD_TOPIC "TOPIC"
# define CMD_NAMES "NAMES"
# define CMD_MSG "PRIVMSG"
# define CMD_PART "PART"

# define PASS_VERIFIED 0b001
# define NICK_VERIFIED 0b010
# define USER_VERIFIED 0b100
# define ALL_VERIFIED 0b111

class Server {
	private:
		int							_port;
		std::string					_password;
		int							_serverSocket;
		struct sockaddr_in			_serverAddress;
		time_t						_created_time;
		char						_message[BUF_SIZE];
		struct timeval				_timeout;
		fd_set						_reads;
		std::vector<struct pollfd>	_poll_fds;
		std::vector<User>			_userList;
		std::vector<Channel>		_channelList;

		void		sendClientMessage(User& user, std::string str);
		void		receiveFirstClientMessage(void);
		void		receiveClientMessage(int clientSocket);
		std::string	concatMessage(int clientSocket);
		void		parseMessageStream(User& user, const std::string& fullMsg);

		void		commandPASS(User& user, std::vector<std::string>& parameters);
		void		commandNICK(User& user, std::vector<std::string>& parameters);
		void		commandUser(User& user, std::vector<std::string>& parameters);

		bool		isChannel(std::string channelName);
		Channel		findChannel(std::string channelName);
		int			getUserIndexByFd(int fd);

		void		commandJOIN(User &user, std::vector<std::string> &parameters);
		void		commandTOPIC(User &user, std::vector<std::string>& parameters);
		void		commandNAMES(User &user, std::vector<std::string>& parameters);
		void		commandMSG(User &user, std::vector<std::string>& parameters);
		void		commandPART(User &user, std::vector<std::string>& parameters);

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
