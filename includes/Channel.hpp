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

# include "irc.hpp"
# include "User.hpp"

# define BOT_CMD_42WORLD "$42world"
# define BOT_CMD_HELP "$help"
# define BOT_CMD_TIME "$time"
# define BOT_CMD_ADD "$add"
# define BOT_CMD_REMOVE "$remove"

class Channel {
	private:
		std::string		_channelName;
		userVector		_channelOperator;	// user의 nickname 저장
		userVector		_channelUser;
		std::string		_channelTopic;
		std::string		_channelMode;
		stringVector	_badWordList;
		bool			_isDeleted;

		std::string		command42WORLD();
		std::string		commandHELP();
		std::string		commandADD(stringVector& parameters);
		std::string		commandREMOVE(stringVector& parameters);
		std::string		commandTIME();

		bool			addBadWord(std::string word);
		bool			removeBadWord(std::string word);

	public:
		Channel(User* user, std::string name);
		~Channel();

		void			joinNewUser(User* user);
		bool			isOperator(std::string nickname);
		bool			isUserInChannel(std::string nickname);

		std::string		getUserList();
		userVector		getOperatorVector();
		userVector		getNormalUserVector();
		std::string		getChannelName();
		std::string		getChannelMode();
		stringVector&	getBadWordList();
		bool			getIsDeleted();

		void			setTopic(std::string topic);
		void			setChannelMode(std::string channelMode);
		void			setIsDeleted(bool isDeleted);

		std::string		parseBotCommand(std::string& message);
		bool			isBadWordIncluded(std::string& message);
		void			deleteNormalUser(std::string nickname);
		int				deleteOperatorUser(std::string nickname);
};

typedef	std::vector<Channel*>			channelVector;
typedef	std::vector<Channel*>::iterator	channelIter;

#endif
