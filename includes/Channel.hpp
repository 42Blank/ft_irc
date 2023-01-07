/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 10:08:13 by san              #+#    #+#             */
/*   Updated: 2023/01/07 01:59:13 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"

# define MODE_O 0x01
# define MODE_P 0x02
# define MODE_S 0x04
# define MODE_I 0x08
# define MODE_T 0x10
# define MODE_N 0x20
# define MODE_M 0x40
# define MODE_L 0x80

class Channel {
	private:
		std::string		_channelName;
		userVector		_channelOperator;	// user의 nickname 저장
		userVector		_channelUser;
		std::string		_channelTopic;
		unsigned char	_channelMode;
		bool			_isDeleted;

	public:
		Channel(User* user, std::string name);
		~Channel();

		void			joinNewUser(User* user);
		void			joinNewOperator(User* user);
		bool			isOperator(std::string nickname);
		bool			isUserInChannel(std::string nickname);

		std::string		getUserList();
		userVector		getOperatorVector();
		userVector		getNormalUserVector();
		std::string		getChannelName();
		std::string		getChannelModeToStr();
		unsigned char	getChannelMode();
		bool			getIsDeleted();

		std::string		modeBitToStr(unsigned char mode);
		unsigned char	modeStrToBit(char m);
		void			changeChannelMode(Channel *ch, std::string &mode);


		void			setTopic(std::string topic);
		void			setChannelMode(unsigned char channelMode);
		void			setIsDeleted(bool isDeleted);

		void			deleteNormalUser(std::string nickname);
		int				deleteOperatorUser(std::string nickname);

};

typedef	std::vector<Channel*>			channelVector;
typedef	std::vector<Channel*>::iterator	channelIter;

#endif
