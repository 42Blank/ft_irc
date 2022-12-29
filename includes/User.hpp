/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 15:55:05 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/29 16:26:12 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>

class User {
	private:
		int					_client_socket; // 클라이언트 소켓 fd
		struct sockaddr_in*	_client_address; // 클라이언트 소켓 정보
		socklen_t*			_client_address_size; // 클라이언트 소켓 정보 크기
		std::string			_nickname;
		std::string			_username;

	public:
		User();
		~User();

		int					getSocketDesc();
		struct sockaddr_in*	getAddressPtr();
		socklen_t*			getAddressSizePtr();
		std::string			getNickname();
		std::string			getUsername();

		void				setNickname(std::string nickname);
		void				setUsername(std::string username);
		void				setSocketDesc(int client_socket);
};

#endif
