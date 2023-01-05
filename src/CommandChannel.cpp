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
	std::vector<Channel>::iterator	iterChannel;

	for (iterChannel = _channelList.begin(); iterChannel < _channelList.end(); iterChannel++) {
		if (!(*iterChannel).getChannelName().compare(channelName)) {
			return *iterChannel;
		}
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
	std::vector<User>::iterator it;

	for (it = _s_userList.begin(); it < _s_userList.end(); it++) {
		if (!(*it).getNickname().compare(nickname)) {
			return *it;
		}
	}
	throw std::runtime_error(Error(ERR_NOSUCHNICK, nickname));
}



void	Server::commandJOIN(User &user, std::vector<std::string> &parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));

	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_JOIN));

	if (isChannel(parameters[1])) { //채널에 가입된다.
		sendMessage(user, " JOIN " + parameters[1]);
		Channel &ch = findChannel(parameters[1]);
		ch.joinNewUser(user);
		sendMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), ch.getUserList()));
		sendMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + ch.getChannelName()));
		sendMessageBroadcast(1, ch, user, "JOIN :" + ch.getChannelName());

	} else {	// 채널을 새로 생성한다.
		sendMessage(user, " JOIN " + parameters[1]);
		Channel ch = Channel(user, parameters[1]);
		_channelList.push_back(ch);
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

	Channel &ch = findChannel(parameters[1]);
	if (ch.isOperator(user.getNickname())) {
		ch.setTopic(channelTopic);
		sendMessage(user, " TOPIC " + channelName + " " + channelTopic);
		sendMessageBroadcast(1, ch, user, "TOPIC " + channelName + " " + channelTopic);
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
		sendMessage(user, receiver, "PRIVMSG " + receiver.getNickname() + " " + ft_getStringAfterColon(parameters));
		// 여기도 공백들어오는 메세지 추가하기

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
			sendMessage(user, Reply(RPL_CHANNELMODEIS, user.getNickname(), ch.getChannelName() + " :" + ch.getModeInServer()));
		} else {
			// 사용자는 타 사용자의 모드를 볼 수 없다.
			if (!parameters[1].compare(user.getNickname()))
				sendMessage(user, Reply(RPL_UMODEIS, user.getNickname() + " :" + user.getModeInServer()));
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
		sendMessage(user, "MODE " + user.getNickname() + " :" + parameters[2]);
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
	sendMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), ch.getUserList()));
	sendMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + ch.getChannelName()));
}
