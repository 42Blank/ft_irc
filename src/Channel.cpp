/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 10:08:55 by san               #+#    #+#             */
/*   Updated: 2023/01/08 04:53:40 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Channel.hpp"

Channel::Channel(User* user, std::string channelName) {
	_channelOperator.push_back(user);
	_channelName = channelName;
	_channelMode = "+nt";
}

Channel::~Channel() {}

// 새로운 유저가 조인하면 이 메서드를 통해서 채널의 userList에 추가한다.
void	Channel::joinNewUser(User* user) {
	_channelUser.push_back(user);
}

// 관리자인지 확인하는 메서드
bool	Channel::isOperator(std::string nickname) {
	userIter	iter;

	for (iter = _channelOperator.begin(); iter < _channelOperator.end(); iter++) {
		if (!((*iter)->getNickname().compare(nickname)))
			return true;
	}
	return false;
}

bool	Channel::isUserInChannel(std::string nickname) {
	userIter	iter;
	for (iter = _channelUser.begin(); iter < _channelUser.end(); iter++) {
		if (!(*iter)->getNickname().compare(nickname))
			return true;
	}
	return false;
}

std::string			Channel::getUserList() {
	std::string	userList;
	userIter	iter;

	userList = "= " + _channelName + " :";
	for (iter = _channelUser.begin(); iter < _channelUser.end(); iter++)
		userList += ((*iter)->getNickname() + " ");
	for (iter = _channelOperator.begin(); iter < _channelOperator.end(); iter++)
		userList += ("@" + (*iter)->getNickname() + " ");
	return userList + BOT_NAME;
}

userVector	Channel::getOperatorVector() {
	return _channelOperator;
}

userVector	Channel::getNormalUserVector() {
	return _channelUser;
}

std::string	Channel::getChannelName() {
	return _channelName;
}

std::string			Channel::getChannelMode() {
	return _channelMode;
}

void	Channel::setTopic(std::string topic) {
	_channelTopic = topic;
}

void	Channel::setChannelMode(std::string channelMode) {
	_channelMode = channelMode;
}

void	Channel::deleteNormalUser(std::string nickname){
	userIter	iter;

	for (iter = _channelUser.begin(); iter < _channelUser.end(); iter++) {
		if (!(*iter)->getNickname().compare(nickname))
			_channelUser.erase(iter);
	}
}

// 0 -> false, 1 -> true  || bool return 으로 바꾸기
int		Channel::deleteOperatorUser(std::string nickname) {
	userIter	iter;

	for (iter = _channelOperator.begin(); iter < _channelOperator.end(); iter++) {
		if (!(*iter)->getNickname().compare(nickname))
			_channelOperator.erase(iter);
	}

	if (_channelOperator.size() == 0) {
		if (!_channelUser.empty()) { //빈 벡터가 아니라면
			User	*user = _channelUser[0];
			_channelOperator.push_back(user);
			_channelUser.erase(_channelUser.begin());
			return 1;
		} else {
			return 0;
		}
	}

	return (_channelOperator.size());
}
