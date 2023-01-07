/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 00:04:29 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/08 04:53:33 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

bool	Server::isServerUser(int clientFd) {
	userIter	iter;

	for (iter = _serverUser.begin(); iter < _serverUser.end(); iter++) {
		if (clientFd == (*iter)->getSocketFd())
			return (true);
	}
	return (false);
}

bool	Server::isServerUser(std::string nickname) {
	userIter	iter;

	for (iter = _serverUser.begin(); iter < _serverUser.end(); iter++) {
		if (!(*iter)->getNickname().compare(nickname))
			return true;
	}
	return false;
}

User*	Server::findUser(int clientFd) {
	userIter	iter;

	for (iter = _serverUser.begin(); iter < _serverUser.end(); iter++) {
		if (clientFd == (*iter)->getSocketFd())
			return (*iter);
	}
	throw std::runtime_error(Error(ERR_CANNOTFINDUSERFD));
}

User*	Server::findUser(std::string nickname) {
	userIter iter;

	for (iter = _serverUser.begin(); iter < _serverUser.end(); iter++) {
		if (!(*iter)->getNickname().compare(nickname))
			return *iter;
	}
	throw std::runtime_error(Error(ERR_NOSUCHNICK, nickname));
}

void	Server::setUserDisconnectByFd(int clientFd) {
	userIter	userIter;

	for (userIter = _serverUser.begin(); userIter < _serverUser.end(); userIter++) {
		if ((*userIter)->getSocketFd() != clientFd) continue;
		(*userIter)->setIsDisconnected(true);
		return;
	}
}

bool	Server::isChannel(std::string channelName) {
	channelIter	iter;

	if (!ft_isValidChannelName(channelName)) return false;
	for (iter = _channelList.begin(); iter < _channelList.end(); iter++)
		if (!((*iter)->getChannelName().compare(channelName))) return true;
	return false;
}

Channel*	Server::findChannel(std::string channelName) {
	channelIter	iter;

	for (iter = _channelList.begin(); iter < _channelList.end(); iter++) {
		if (!((*iter)->getChannelName().compare(channelName)))
			return (*iter);
	}
	throw std::runtime_error(Error(ERR_NOSUCHCHANNEL, channelName));
}

void	Server::kickUserFromChannel(Channel* ch, User* user, std::string message) {
	sendMessageBroadcast(0, ch, user, "KICK " + ch->getChannelName() + " " + user->getNickname() + ":" + message);
	user->deleteJoinedChannel(ch->getChannelName());
	ch->deleteNormalUser(user->getNickname());
	if (ch->deleteOperatorUser(user->getNickname()))
		sendMessageBroadcast(0, ch, user, "MODE " + ch->getChannelName() + " +o :" + ch->getOperatorVector()[0]->getNickname());
	else {
		// 채널 지우기
	}
}
