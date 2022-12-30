/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 15:55:05 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/30 23:13:19 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class User {
	private:
		int					_clientSocket; // 클라이언트 소켓 fd
		struct sockaddr_in*	_clientAddress; // 클라이언트 소켓 정보
		socklen_t			_clientAddressSize; // 클라이언트 소켓 정보 크기
		std::string			_nickname;
		std::string			_username;
		// bool				_isAdmin;

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
		void				setSocketDesc(int clientSocket);
};

#endif
