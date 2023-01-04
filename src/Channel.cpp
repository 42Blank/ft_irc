
#include "../includes/Channel.hpp"

// 여기 유저 nickname만 받아서 Channel user list에 넣으면 안될지 고민 중
Channel::Channel(User &user, std::string name) {
	_operator = user;
	// _c_userList.push_back(user);
	_channelName = name;
}

Channel::~Channel() {

}

std::string			Channel::getTopic() {
	return _topic;
}

// 그냥 유저 닉네임을 모두 출력하도록 함
std::string			Channel::getUserList() {

	std::string	userList;
	std::vector<User>::iterator	iter;

	userList = "= " + _channelName + " :";
	for (iter = _c_userList.begin(); iter < _c_userList.end(); iter++)
		userList += ((*iter).getNickname() + " ");
	userList += "@" + _operator.getNickname();
	return userList;
}

std::string			Channel::getChannelName() {
	return _channelName;
}

void				Channel::setTopic(std::string topic) {
	_topic = topic;
}

// 새로운 유저가 조인하면 이 메서드를 통해서 채널의 userList에 추가한다.
void	Channel::joinNewUser(User user) {
	_c_userList.push_back(user);
}

// 관리자인지 확인하는 메서드
bool	Channel::isOperator(User user) {
	if (user.getNickname().compare(_operator.getNickname()) == 0) {
		return true;
	}
	return false;
}

bool	Channel::isUser(std::string nickname) {

	std::vector<User>::iterator	iter;

	for (iter = _c_userList.begin(); iter < _c_userList.end(); iter++) {
		if ((*iter).getNickname().compare(nickname) == 0)
			return true;
	}
	return false;
}

void	Channel::deleteNormalUser(std::string nickname){
	std::vector<User>::iterator	iter;

	for (iter = _c_userList.begin(); iter < _c_userList.end(); iter++) {
		if ((*iter).getNickname().compare(nickname) == 0)
			_c_userList.erase(iter);
	}
}

void	Channel::deleteOperatorUser(std::string nickname) {
	// Operator 에 벡터의 첫번쨰 사용자 넣고 만일 벡터에 아무도 없으면 채널이 사라지도록 하기 
(void)nickname;
}
