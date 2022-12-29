/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 16:35:50 by san               #+#    #+#             */
/*   Updated: 2022/12/30 03:51:41 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "User.hpp"

# define BUF_SIZE 1024

class Server {
	private:
		int					_server_socket;
		struct sockaddr_in	_server_address;
		std::vector<User *>	_user_vector;
		char				_message[BUF_SIZE];

		void		connectWithClient();
		std::string	concatMessage(int client_socket);
		void		parseUserData(User* user, const std::string& fullMsg);


	public:
		Server();
		Server(char *port);// 비번 추가 해야 함.
		~Server();

		void	serverOn(void);
		void	serverOff(void);

};

#endif
