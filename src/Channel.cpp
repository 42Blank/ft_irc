/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 10:08:55 by san               #+#    #+#             */
/*   Updated: 2023/01/07 15:41:26 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(User* user, std::string channelName) {
	_channelOperator.push_back(user);
	_channelName = channelName;
	_channelMode = 0x30;
	// _channelMode = 0;
	_isDeleted = false;
}

Channel::~Channel() {}

// 새로운 유저가 조인하면 이 메서드를 통해서 채널의 userList에 추가한다.
void	Channel::joinNewUser(User* user) {
	_channelUser.push_back(user);
}

void	Channel::joinNewOperator(User* user) {
	_channelOperator.push_back(user);
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
	return userList;
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

std::string	Channel::getChannelModeToStr() {
	return modeBitToStr(_channelMode);
}

unsigned char	Channel::getChannelMode() {
	return _channelMode;
}

bool	Channel::getIsDeleted() {
	return _isDeleted;
}

std::string	Channel::modeBitToStr(unsigned char channelMode) {
	std::string	mode = "+";

	if (channelMode & MODE_O)	mode.push_back('o');
	if (channelMode & MODE_P)	mode.push_back('p');
	if (channelMode & MODE_S)	mode.push_back('s');
	if (channelMode & MODE_I)	mode.push_back('i');
	if (channelMode & MODE_T)	mode.push_back('t');
	if (channelMode & MODE_N)	mode.push_back('n');
	if (channelMode & MODE_M)	mode.push_back('m');
	if (channelMode & MODE_L)	mode.push_back('l');
	
	return mode;
}

unsigned char	Channel::modeStrToBit(char m) {
	if (m == 'o') return MODE_O;
	else if (m == 'p') return MODE_P;
	else if (m == 's') return MODE_S;
	else if (m == 'i') return MODE_I;
	else if (m == 't') return MODE_T;
	else if (m == 'n') return MODE_N;
	else if (m == 'm') return MODE_M;
	else return MODE_L;
	// else if (m == 'l') return MODE_L;
}

void	Channel::changeChannelMode(Channel *ch, std::string &mode) {
	int len = (int)(mode.length());

	if (mode[0] == '+') {
		for (int i = 1; i < len; i++)
			ch->setChannelMode(ch->getChannelMode() | ch->modeStrToBit(mode[i]));
	} else if (mode[0] == '-') {
		for (int i = 1; i < len; i++)
			ch->setChannelMode(ch->getChannelMode() & ch->modeStrToBit(mode[i]));
	} else {}
		// throw std::runtime_error(Error());
}

void	Channel::setTopic(std::string topic) {
	_channelTopic = topic;
}

void	Channel::setChannelMode(unsigned char channelMode) {
	_channelMode = channelMode;
}

void	Channel::setIsDeleted(bool isDeleted) {
	_isDeleted = isDeleted;
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
			joinNewOperator(user);
			deleteNormalUser(user->getNickname());
			return 1;
		} else {
			return 0;
		}
	}




	return (_channelOperator.size());
}
