/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasong <jasong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 15:55:05 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/06 06:16:02 by jasong           ###   ########.fr       */
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
		std::string					_modeInServer;
	public:
		User();
		User(const User& instance);
		~User();
		User&				operator=(const User& instance);

		int							getSocketFd() const;
		struct sockaddr_in*			getAddressPtr() const;
		socklen_t*					getAddressSizePtr() const;
		std::string					getNickname() const;
		std::string					getUsername() const;
		std::string					getHostname() const;
		short						getIsVerified() const;
		bool						getIsDisconnected() const;
		std::string					getModeInServer() const;
		std::vector<std::string>	getChannelList() const;

		void						setNickname(std::string nickname);
		void						setUsername(std::string username);
		void						setHostname(std::string hostname);
		void						setIsVerified(short what);
		void						setSocketFd(int clientFd);
		void						setIsDisconnected(bool isDisconnected);
		void						setModeInServer(std::string mode);
		void						setChannelList(std::vector<std::string> channelList);

		void						addJoinedChannelByName(std::string channelName);
		void						deleteJoinedChannelByName(std::string channelName);

};

std::ostream&	operator<<(std::ostream& out, const User& instance);

#endif
