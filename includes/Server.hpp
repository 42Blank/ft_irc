/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2022/12/30 04:27:24 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "User.hpp"
# include "Error.hpp"
# include "Command.hpp"
# include <sys/select.h>
# include <sys/time.h>

# define BUF_SIZE 1024

class Server {
	private:
		int					_server_socket;
		struct sockaddr_in	_server_address;
		std::vector<User *>	_user_vector;
		char				_message[BUF_SIZE];
		struct timeval		_timeout;
		fd_set				_reads;

		void		receiveClientMessage();
		std::string	concatMessage(int client_socket);
		void		parseMessageStream(User* user, const std::string& fullMsg);

		void		sendToClientMessage(User *user);
		void		socketMultiPlex();

	public:
		Server();
		Server(char *port);// 비번 추가 해야 함.
		~Server();

		void	serverOn(void);
		void	serverOff(void);

};

#endif
