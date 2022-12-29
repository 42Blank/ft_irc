/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 15:55:05 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/29 16:04:26 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>

class User {
	private:
		std::string			_nickname;
		std::string			_username;
		int					_client_socket; // 클라이언트 소켓 fd
		struct sockaddr_un	_client_address; // 클라이언트 소켓 정보
		socklen_t			_client_address_size; // 클라이언트 소켓 정보 크기

	public:
		User();
		~User();
		void				setNames(std::string _nickname, std::string _username);
};

#endif
