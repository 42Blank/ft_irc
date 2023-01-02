#include "../includes/Server.hpp"
#include "../includes/Reply.hpp"


#include <iostream>

void	Server::commandJOIN(User &user, std::vector<std::string> &parameters) {

	std::cerr << "in\n";
	//ERR_NEEDMOREPARAMS (461)
	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_JOIN));

	if (isChannel(parameters[1])) {
		//채널에 가입된다.
		findChannel(parameters[1]).joinNewUser(user);
		sendClientMessage(user, user.getNickname() + " JOIN " + parameters[1]);
		Channel channel = findChannel(parameters[1]);
		if (channel.getTopic() == "")
			sendClientMessage(user, Reply(RPL_NOTOPIC, channel.getChannelName()));
		else
			sendClientMessage(user, Reply(RPL_TOPIC, channel.getChannelName(), ":" + channel.getTopic()));
		std::cerr << "send\n";
		sendClientMessage(user, Reply(RPL_NAMREPLY, channel.getChannelName(), channel.getUserList()));
		sendClientMessage(user, Reply(RPL_ENDOFNAMES, channel.getChannelName()));
		std::cerr << "send2\n";

	} else {
		Channel channel = Channel(user, parameters[1]);
		_channelList.push_back(channel);
		std::cerr << "send\n";
		sendClientMessage(user, Reply(RPL_NOTOPIC, channel.getChannelName()));
		std::cerr << "send2\n";
		sendClientMessage(user, Reply(RPL_NAMREPLY, channel.getChannelName(), channel.getUserList()));
		std::cerr << "send3\n";
		sendClientMessage(user, Reply(RPL_ENDOFNAMES, channel.getChannelName()));
	}

}

Channel		Server::findChannel(std::string channelName) {
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

	// channelName = channelName.substr(1, channelName.npos);
	//순회하면서 해당 이름 채널이 있는지 확인
	std::vector<Channel>::iterator	iter;
	for (iter = _channelList.begin(); iter < _channelList.end(); iter++) {
		if ((*iter).getChannelName().compare(channelName) == 0)
			return true;
	}
	return false;
}

//관리자만 사용할 수 있다.
void		Server::commandTOPIC(User &user, std::vector<std::string>& parameters) {
	if (parameters.size() < 3) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_TOPIC));

	std::string channelName = parameters[1];
	if (!isChannel(channelName)) throw std::runtime_error(Error(ERR_NOSUCHCHANNEL, channelName));

	std::string	topic;
	std::vector<std::string>::iterator	iter;
	for (iter = parameters.begin() + 2; iter < parameters.end(); iter++) {
		topic = topic + *iter;
	}

	std::vector<Channel>::iterator	iterChannel;
	for (iterChannel = _channelList.begin(); iterChannel < _channelList.end(); iterChannel++) {
		if ((*iterChannel).getChannelName().compare(channelName) == 0) {
			if ((*iterChannel).isOperator(user))
				(*iterChannel).setTopic(topic);
			else
				throw std::runtime_error(Error(ERR_CHANOPRIVSNEEDED, channelName));
		}
	}

}

void		Server::commandMSG(User &user, std::vector<std::string>& parameters) {

(void)user;
(void)parameters;

}

void		Server::commandPART(User &user, std::vector<std::string>& parameters) {

(void)user;
(void)parameters;
}


void		Server::commandNAMES(User &user, std::vector<std::string>& parameters) {

(void)user;
(void)parameters;

}

// void	Server::commandUser(User* user, std::vector<std::string>& parameters) {


// 	sendClientMessage(user,
// 		":127.0.0.1 001 " + user->getNickname() + " :\033[1;32mWelcome to the " + SERVER_NAME + "\e[0m " + \
// 		user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname()
// 	);
// }
