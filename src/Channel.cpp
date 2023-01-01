
#include "../includes/Channel.hpp"

// 여기 유저 nickname만 받아서 Channel user list에 넣으면 안될지 고민 중 
Channel::Channel(User &user) {
	_operator = user.getNickname();
	_userList.push_back(user);
}

Channel::~Channel() {

}

std::string			Channel::getTopic() {
	return _topic;
}

std::vector<User>	Channel::getuserList() {
	return _userList;
}

void				Channel::setTopic(std::string topic) {
	_topic = topic;
}

// 새로운 유저가 조인하면 이 메서드를 통해서 채널의 userList에 추가한다. 
void	Channel::joinNewUser(User user) {
	_userList.push_back(user);
}

// 관리자인지 확인하는 메서드 
bool	Channel::isOperator(User user) {
	if (user.getNickname().compare(_operator) == 0) {
		return true;
	}
	return false;
}

void				Channel::deleteNormalUser(std::string nickname){
	std::vector<User>::iterator	iter;

	for (iter = _userList.begin(); iter < _userList.end(); iter++) {
		if ((*iter).getNickname().compare(nickname) == 0)
			_userList.erase(iter);
	}
}

void				Channel::deleteOperatorUser(std::string nickname) {
	
}