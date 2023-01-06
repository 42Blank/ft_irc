/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 15:55:05 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/07 01:57:28 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"
# include <vector>
class User {
	private:
		int							_clientFd; // 클라이언트 소켓 fd
		struct sockaddr_in*			_clientAddress; // 클라이언트 소켓 정보
		socklen_t*					_clientAddressSize; // 클라이언트 소켓 정보 크기
		std::string					_nickname;
		std::string					_username;
		std::string					_hostname;
		std::vector<std::string>	_channelList;
		short						_isVerified;
		bool						_isDisconnected;
		std::string					_userMode;
	public:
		User();
		User(const User& instance);
		~User();
		User&						operator=(const User& instance);

		int							getSocketFd() const;
		struct sockaddr_in*			getAddressPtr() const;
		socklen_t*					getAddressSizePtr() const;
		std::string					getNickname() const;
		std::string					getUsername() const;
		std::string					getHostname() const;
		short						getIsVerified() const;
		bool						getIsDisconnected() const;
		std::string					getUserMode() const;
		std::vector<std::string>&	getChannelList();

		void						setNickname(std::string nickname);
		void						setUsername(std::string username);
		void						setHostname(std::string hostname);
		void						setIsVerified(short what);
		void						setSocketFd(int clientFd);
		void						setIsDisconnected(bool isDisconnected);
		void						setUserMode(std::string userMode);

		void						addJoinedChannel(std::string channelName);
		void						deleteJoinedChannel(std::string channelName);

};

std::ostream&	operator<<(std::ostream& out, const User& instance);

#endif
