#include "../includes/Server.hpp"
#include "../includes/Reply.hpp"


#include <iostream>

bool		Server::isChannel(std::string channelName) {
	if (channelName[0] != '&' && channelName[0] != '#' && channelName[0] != '+' && channelName[0] != '!')
		return false;
	std::vector<Channel>::iterator	iter;
	for (iter = _channelList.begin(); iter < _channelList.end(); iter++) {
		if ((*iter).getChannelName().compare(channelName) == 0)
			return true;
	}
	return false;
}

Channel		&Server::findChannel(std::string channelName) {
	std::vector<Channel>::iterator	iterChannel;

	for (iterChannel = _channelList.begin(); iterChannel < _channelList.end(); iterChannel++) {
		if ((*iterChannel).getChannelName().compare(channelName) == 0) {
			return *iterChannel;
		}
	}
	throw std::runtime_error(Error(ERR_NOSUCHCHANNEL, channelName));
}

bool		Server::isServerUser(std::string nickname) {

	std::vector<User>::iterator	iter;

	for (iter = _s_userList.begin(); iter < _s_userList.end(); iter++) {
		if ((*iter).getNickname().compare(nickname) == 0)
			return true;
	}
	return false;
}

User		&Server::findUser(std::string nickname) {
	std::vector<User>::iterator it;

	for (it = _s_userList.begin(); it < _s_userList.end(); it++) {
		if ((*it).getNickname().compare(nickname) == 0) {
			return *it;
		}
	}
	throw std::runtime_error(Error(ERR_NOSUCHNICK, nickname));
}



void	Server::commandJOIN(User &user, std::vector<std::string> &parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));

	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_JOIN));

	if (isChannel(parameters[1])) { //채널에 가입된다.
		sendClientMessage(user, " JOIN " + parameters[1]);
		Channel &ch = findChannel(parameters[1]);
		ch.joinNewUser(user);
		sendClientMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), ch.getUserList()));
		sendClientMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + ch.getChannelName()));
		sendMessageBroadcast(1, ch, user, "JOIN :" + ch.getChannelName());

	} else {	// 채널을 새로 생성한다.
		sendClientMessage(user, " JOIN " + parameters[1]);
		Channel ch = Channel(user, parameters[1]);
		_channelList.push_back(ch);
		sendClientMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), ch.getUserList()));
		sendClientMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + ch.getChannelName()));
	}
}


//관리자만 사용할 수 있다.
void		Server::commandTOPIC(User &user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 3) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_TOPIC));

	std::string channelName = parameters[1];
	if (!isChannel(channelName)) throw std::runtime_error(Error(ERR_NOSUCHCHANNEL, channelName));

	std::string	topic;
	std::vector<std::string>::iterator	iter;
	///////////여기 공백들어가는 ft_split 어떻게 쓰는지 물어보기 공백뒤 토픽 짤림.
	for (iter = parameters.begin() + 2; iter < parameters.end(); iter++) {
		topic = topic + *iter;
	}
	topic = parameters[2];

	Channel &ch = findChannel(parameters[1]);
	if (ch.isOperator(user.getNickname())) {
		ch.setTopic(topic);
		sendClientMessage(user, " TOPIC " + parameters[1] + " " + parameters[2]);
		sendMessageBroadcast(1, ch, user, "TOPIC " + ch.getChannelName() + " " + ch.getTopic());
	} else {
		throw std::runtime_error(Error(ERR_CHANOPRIVSNEEDED, channelName));
	}
}

void		Server::commandMSG(User &user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	
	if (isChannel(parameters[1])) {
		Channel &ch = findChannel(parameters[1]);
		sendMessageBroadcast(1, ch, user, "PRIVMSG " + ch.getChannelName() + " " + ft_getStringAfterColon(parameters));

	} else if (isServerUser(parameters[1])) {
		User	&receiver = findUser(parameters[1]);
		sendMessageUnicast(user, receiver, "PRIVMSG " + receiver.getNickname() + " " + ft_getStringAfterColon(parameters));
	} else {
		// 채널도 사용자도 아닌 에러
	}

}

/*
MODE
인자가 두개 오면 해당 타겟의 모드 상태를 알려준다.
인자가 세개 오면 해당 타겟의 모드를 3번째 인자로 바꿔달라는 요청이다. 
*/
void		Server::commandMODE(User &user, std::vector<std::string>& parameters) {

	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS));
	if (!isServerUser(parameters[1]) && !isChannel(parameters[1])) throw std::runtime_error(Error(ERR_NOSUCHNICK));
	
	if (parameters.size() == 2) {
		if (isChannel(parameters[1])) {
			Channel	&ch = findChannel(parameters[1]);
			sendClientMessage(user, Reply(RPL_CHANNELMODEIS, user.getNickname(), ch.getChannelName() + " :" + ch.getModeInServer()));
		} else {
			// 사용자는 타 사용자의 모드를 볼 수 없다. 
			if (parameters[1].compare(user.getNickname()) == 0)
				sendClientMessage(user, Reply(RPL_UMODEIS, user.getNickname() + " :" + user.getModeInServer()));
			else 
				throw std::runtime_error(Error(ERR_USERSDONTMATCH, user.getNickname()));
		}
	} else if (parameters.size() == 3) {
		if (isChannel(parameters[1])) {
			Channel	&ch = findChannel(parameters[1]);
			ch.setModeInServer(parameters[2]);
		} else {
			user.setModeInServer(parameters[2]);
		}
		sendClientMessage(user, "MODE " + user.getNickname() + " :" + parameters[2]);
	}
}

void		Server::commandPART(User &user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));

	if (isChannel(parameters[1])) {	// 채널이면
		Channel	&ch = findChannel(parameters[1]);
		if (ch.isUser(user.getNickname())) { // 유저가 있으면
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
	sendClientMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), ch.getUserList()));
	sendClientMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + ch.getChannelName()));
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
	ch.deleteNormalUser(parameters[2]);
	ch.deleteOperatorUser(parameters[2]);
}
