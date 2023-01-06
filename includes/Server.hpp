/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2023/01/07 00:05:01 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "User.hpp"
# include "Reply.hpp"
# include "Channel.hpp"

# define BUF_SIZE 1024
# define SERVER_NAME "SJJS_IRCServ"

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
		char						_message[BUF_SIZE];
		struct timeval				_timeout;
		fd_set						_reads;
		std::vector<struct pollfd>	_poll_fds;
		std::vector<User>			_s_userList;
		std::vector<Channel>		_channelList;

		void		acceptClient(void);

		void		sendMessage(User& user, std::string str);
		void		sendMessage(User& sender, User& receiver, std::string str);
		void		sendMessageBroadcast(int mode, Channel& ch, User& sender, std::string str);

		void		receiveFirstClientMessage(int clientFd);
		void		receiveClientMessage(int clientSocket);
		std::string	concatMessage(int clientSocket);
		void		parseMessageStream(User& user, const std::string& fullMsg);
		void		setUserDisconnectByFd(int client_fd);
		void		disconnectClients();

		void		commandCAP(User& user, std::vector<std::string>& parameters);
		void		commandPASS(User& user, std::vector<std::string>& parameters);
		void		commandNICK(User& user, std::vector<std::string>& parameters);
		void		commandUSER(User& user, std::vector<std::string>& parameters);
		void		commandPING(User& user, std::vector<std::string>& parameters);
		void		commandPONG(User& user, std::vector<std::string>& parameters);
		void		commandJOIN(User &user, std::vector<std::string> &parameters);
		void		commandTOPIC(User &user, std::vector<std::string>& parameters);
		void		commandNAMES(User &user, std::vector<std::string>& parameters);
		void		commandMSG(User &user, std::vector<std::string>& parameters);
		void		commandMODE(User &user, std::vector<std::string>& parameters);
		void		commandPART(User &user, std::vector<std::string>& parameters);
		void		commandQUIT(User& user, std::vector<std::string>& parameters);
		void		commandKICK(User& user, std::vector<std::string>& parameters);
		void		commandWHO(User& user, std::vector<std::string>& parameters);

		bool		isChannel(std::string channelName);
		Channel		&findChannel(std::string channelName);
		bool		isServerUser(std::string nickname);
		bool		isServerUser(int socketFd);
		User		&findUser(std::string nickname);
		User		&getUserByFd(int fd);

		void		checkIsVerified(User& user);

	public:
		Server(char* port, char* password);
		~Server();

		void		serverOn(void);
		void		serverOff(void);
};

#endif
