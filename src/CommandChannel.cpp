#include "../includes/Server.hpp"
#include "../includes/Reply.hpp"


#include <iostream>

void	Server::commandJOIN(User &user, std::vector<std::string> &parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));

	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_JOIN));

	if (isChannel(parameters[1])) { //채널에 가입된다.
		sendClientMessage(user, " JOIN :" + parameters[1]);
		Channel &channel = findChannel(parameters[1]);
		channel.joinNewUser(user);
		std::cerr << "send join channel\n";
		sendClientMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), channel.getUserList()));
		sendClientMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + channel.getChannelName()));
	} else {
		sendClientMessage(user, " JOIN :" + parameters[1]);
		Channel channel = Channel(user, parameters[1]);
		_channelList.push_back(channel);
		std::cerr << "make new channel\n";
		sendClientMessage(user, Reply(RPL_NAMREPLY, user.getNickname(), channel.getUserList()));
		sendClientMessage(user, Reply(RPL_ENDOFNAMES, user.getNickname() + " " + channel.getChannelName()));
	}
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

//관리자만 사용할 수 있다.
void		Server::commandTOPIC(User &user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 3) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_TOPIC));

	std::string channelName = parameters[1];
	if (!isChannel(channelName)) throw std::runtime_error(Error(ERR_NOSUCHCHANNEL, channelName));

	// if (parameters.size() == )

	std::string	topic;
	std::vector<std::string>::iterator	iter;
	for (iter = parameters.begin() + 2; iter < parameters.end(); iter++) {
		topic = topic + *iter;
	}

	Channel &ch = findChannel(parameters[1]);
	if (ch.isOperator(user.getNickname())) {
		ch.setTopic(topic);
		sendClientMessage(user, Reply(RPL_TOPIC, user.getNickname() + " " + ch.getChannelName(), ch.getTopic()));
	} else {
		throw std::runtime_error(Error(ERR_CHANOPRIVSNEEDED, channelName));
	}
	////////////debug
	Channel &ch1 = findChannel(parameters[1]);
	std::cerr << ch1.getTopic() << "\n";

}

void		Server::commandMSG(User &user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));


	sendClientMessage(user,  + " PRIVMSG " + parameters[1] + " :" + parameters[2]);
// 	if (isChannel(parameters[1])) {
// 		Channel &ch = findChannel(parameters[1]);


// 	} else if (isUser(parameters[1])) {

// 	} else {
// 		// 채널도 사용자도 아닌 에러
// 	}

// (void)user;
// (void)parameters;

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
			std::cout << "sdfn\n";
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
			sendClientMessage(user, "PART " + ch.getChannelName() + ft_getStringAfterColon(parameters));
			ch.deleteNormalUser(user.getNickname());
		} else if (ch.isOperator(user.getNickname())) {
			sendClientMessage(user, "PART " + ch.getChannelName() + ft_getStringAfterColon(parameters));
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

