/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 10:08:55 by san               #+#    #+#             */
/*   Updated: 2023/01/07 02:13:14 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(User &user, std::string name) {
	_operator.push_back(user);
	// _c_userList.push_back(user);
	_channelName = name;
	_channelMode = "+nt";
}

Channel::~Channel() {}

// 새로운 유저가 조인하면 이 메서드를 통해서 채널의 userList에 추가한다.
void	Channel::joinNewUser(User user) {
	_c_userList.push_back(user);
}

// 관리자인지 확인하는 메서드
bool	Channel::isOperator(std::string nickname) {
	userIter	iter;
	for (iter = _operator.begin(); iter < _operator.end(); iter++) {
		if (!((*iter).getNickname().compare(nickname)))
			return true;
	}
	return false;
}

bool	Channel::isUser(std::string nickname) {
	userIter	iter;
	for (iter = _c_userList.begin(); iter < _c_userList.end(); iter++) {
		if (!(*iter).getNickname().compare(nickname))
			return true;
	}
	return false;
}

std::string			Channel::getTopic() {
	return _topic;
}

// 그냥 유저 닉네임을 모두 출력하도록 함
std::string			Channel::getUserList() {
	std::string	userList;
	userIter	iter;

	userList = "= " + _channelName + " :";
	for (iter = _c_userList.begin(); iter < _c_userList.end(); iter++)
		userList += ((*iter).getNickname() + " ");
	for (iter = _operator.begin(); iter < _operator.end(); iter++)
		userList += ("@" + (*iter).getNickname() + " ");
	return userList;
}

std::vector<User>	Channel::getOperatorVector() {
	return _operator;
}

std::vector<User>	Channel::getNormalUserVector() {
	return _c_userList;
}

std::string			Channel::getChannelName() {
	return _channelName;
}

std::string			Channel::getChannelMode() {
	return _channelMode;
}

void				Channel::setTopic(std::string topic) {
	_topic = topic;
}

void				Channel::setChannelMode(std::string channelMode) {
	_channelMode = channelMode;
}

void	Channel::deleteNormalUser(std::string nickname){
	userIter	iter;

	for (iter = _c_userList.begin(); iter < _c_userList.end(); iter++) {
		if (!(*iter).getNickname().compare(nickname))
			_c_userList.erase(iter);
	}
}

int		Channel::deleteOperatorUser(std::string nickname) {
	// Operator 에 벡터의 첫번쨰 사용자 넣고 만일 벡터에 아무도 없으면 채널이 사라지도록 하기
	userIter	iter;

	for (iter = _operator.begin(); iter < _operator.end(); iter++) {
		if (!(*iter).getNickname().compare(nickname))
			_operator.erase(iter);
	}
	return (_operator.size());
}
