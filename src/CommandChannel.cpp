/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandChannel.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 17:53:57 by san               #+#    #+#             */
/*   Updated: 2023/01/07 01:56:20 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Reply.hpp"

#include <iostream>

bool		Server::isChannel(std::string channelName) {
	if (channelName[0] != '&' && channelName[0] != '#' && channelName[0] != '+' && channelName[0] != '!')
		return false;
	std::vector<Channel>::iterator	iter;
	for (iter = _channelList.begin(); iter < _channelList.end(); iter++) {
		if (!(*iter).getChannelName().compare(channelName))
			return true;
	}
	return false;
}

Channel		&Server::findChannel(std::string channelName) {
	std::vector<Channel>::iterator	iter;

	for (iter = _channelList.begin(); iter < _channelList.end(); iter++) {
		if (!(*iter).getChannelName().compare(channelName))
			return *iter;
	}
	throw std::runtime_error(Error(ERR_NOSUCHCHANNEL, channelName));
}

bool		Server::isServerUser(std::string nickname) {
	std::vector<User>::iterator	iter;

	for (iter = _s_userList.begin(); iter < _s_userList.end(); iter++) {
		if (!(*iter).getNickname().compare(nickname))
			return true;
	}
	return false;
}

User		&Server::findUser(std::string nickname) {
	std::vector<User>::iterator iter;

	for (iter = _s_userList.begin(); iter < _s_userList.end(); iter++) {
		if (!(*iter).getNickname().compare(nickname))
			return *iter;
	}
	throw std::runtime_error(Error(ERR_NOSUCHNICK, nickname));
}

void	Server::commandJOIN(User &user, std::vector<std::string> &parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_JOIN));

	std::string	channelName = parameters[1];
	bool	isChannelAvailable = isChannel(channelName);

	sendMessage(user, " JOIN " + channelName);
	if (isChannelAvailable) {
		Channel& ch = findChannel(channelName);
		ch.joinNewUser(user);
		user.addJoinedChannel(channelName);
		sendMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), ch.getUserList()));
		sendMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + ch.getChannelName()));
		sendMessageBroadcast(1, ch, user, "JOIN :" + ch.getChannelName());
	}
	else {
		Channel ch = Channel(user, channelName);
		_channelList.push_back(ch);
		user.addJoinedChannel(channelName);
		std::cout << "joined channel - " << channelName << '\n';
		sendMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), ch.getUserList()));
		sendMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + ch.getChannelName()));
	}
}

//관리자만 사용할 수 있다.
void		Server::commandTOPIC(User &user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 3) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_TOPIC));

	std::string channelName = parameters[1];
	std::string channelTopic = ft_getStringAfterColon(parameters);
	if (!isChannel(channelName)) throw std::runtime_error(Error(ERR_NOSUCHCHANNEL, channelName));

	Channel &ch = findChannel(channelName);
	if (!ch.isOperator(user.getNickname()))
		throw std::runtime_error(Error(ERR_CHANOPRIVSNEEDED, channelName));
	ch.setTopic(channelTopic);
	sendMessage(user, " TOPIC " + channelName + " " + channelTopic);
	sendMessageBroadcast(1, ch, user, "TOPIC " + channelName + " " + channelTopic);
}

void		Server::commandMSG(User &user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));

	std::string name = parameters[1];

	if (isChannel(name)) {
		Channel &ch = findChannel(name);

		if (ch.isUser(user.getNickname()) || ch.isOperator(user.getNickname()))
			sendMessageBroadcast(1, ch, user, "PRIVMSG " + ch.getChannelName() + " " + ft_getStringAfterColon(parameters));
		return;
	}
	if (isServerUser(name)) {
		User	&receiver = findUser(name);
		sendMessage(user, receiver, "PRIVMSG " + name + " " + ft_getStringAfterColon(parameters));
		return;
	}
	throw std::runtime_error(Error(ERR_NOSUCHNICK));
}

/*
MODE
인자가 두개 오면 해당 타겟의 모드 상태를 알려준다.
인자가 세개 오면 해당 타겟의 모드를 3번째 인자로 바꿔달라는 요청이다.
*/
void		Server::commandMODE(User &user, std::vector<std::string>& parameters) {
	int paramLen = parameters.size();
	if (paramLen < 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS));

	std::string	name = parameters[1];
	if (!isServerUser(name) && !isChannel(name)) throw std::runtime_error(Error(ERR_NOSUCHNICK));

	if (paramLen == 2) {
		if (isChannel(name)) {
			Channel	&ch = findChannel(name);
			sendMessage(user, Reply(RPL_CHANNELMODEIS, user.getNickname(), ch.getChannelName() + " :" + ch.getChannelMode()));
			return;
		}
		if (!name.compare(user.getNickname()))
			sendMessage(user, Reply(RPL_UMODEIS, user.getNickname() + " :" + user.getUserMode()));
		else
			throw std::runtime_error(Error(ERR_USERSDONTMATCH, user.getNickname()));
		return;
	}
	if (paramLen == 3) {
		if (isChannel(name)) {
			Channel	&ch = findChannel(name);
			ch.setChannelMode(parameters[2]);
		} else
			user.setUserMode(parameters[2]);
		sendMessage(user, "MODE " + user.getNickname() + " :" + parameters[2]);
	}
}

void		Server::commandPART(User &user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));

	if (isChannel(parameters[1])) {	// 채널이면
		Channel	&ch = findChannel(parameters[1]);
		user.deleteJoinedChannel(ch.getChannelName());
		std::cout << "deleted user channel - " << ch.getChannelName() << '\n';
		if (ch.isUser(user.getNickname())) {
			sendMessageBroadcast(0, ch, user, "PART :" + ch.getChannelName());
			ch.deleteNormalUser(user.getNickname());
		} else if (ch.isOperator(user.getNickname())) {
			sendMessageBroadcast(0, ch, user, "PART :" + ch.getChannelName());
			ch.deleteOperatorUser(user.getNickname());
		} else {
			throw std::runtime_error(Error(ERR_NOTONCHANNEL));
		}
	}
}

void		Server::commandNAMES(User &user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));

	Channel	&ch = findChannel(parameters[1]);
	sendMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), ch.getUserList()));
	sendMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + ch.getChannelName()));
}

// KICK #xx tes1 :
// KICK <channel> <username> : <msg>
// sender가 operator인지 확인
// 채널이 존재하는지 확인
// 채널에 유저가 존재하는지 확인
// kick
void		Server::commandKICK(User &user, std::vector<std::string>& parameters) {
	Channel &ch = findChannel(parameters[1]);

	if (!ch.isOperator(user.getNickname()))
		throw std::runtime_error(Error(ERR_CHANOPRIVSNEEDED));
	if (!ch.isUser(parameters[2]))
		throw std::runtime_error(Error(ERR_NOSUCHNICK));
	sendMessageBroadcast(0, ch, user, "KICK " + ch.getChannelName() + " " + parameters[2] + ":" + ft_getStringAfterColon(parameters));
	User&	kickedUser = findUser(parameters[2]);
	kickedUser.deleteJoinedChannel(parameters[1]);
	ch.deleteNormalUser(parameters[2]);
	ch.deleteOperatorUser(parameters[2]);
}
