

#include "../includes/Server.hpp"
#include "../includes/Reply.hpp"




void	Server::commandJOIN(User *user, std::vector<std::string> &parameters) {

	//ERR_NEEDMOREPARAMS (461)
	if (parameters.size() < 2) throw Error::AuthorizeException(); // TODO: 다른 오류(파라미터 부족)로 교체

	if (isChannel(parameters[1])) {
		//채널에 가입된다. 
		findChannel(parameters[1]).joinNewUser(*user);
		sendClientMessage(user, user->getNickname() + " JOIN " + parameters[1]);
		Channel ch = findChannel(parameters[1]);
		if (ch.getTopic() == "")
			sendClientMessage(user, RPL_NOTOPIC(ch.getChannelName()));
		else
		sendClientMessage(user, RPL_TOPIC(ch.getChannelName(), ch.getTopic()));
		sendClientMessage(user, RPL_NAMREPLY(ch.getChannelName(), ch.getuserList()));
		sendClientMessage(user, RPL_ENDOFNAMES(ch.getChannelName()));
	} else {
		Channel channel = Channel(*user, parameters[1]);
		_channelList.push_back(channel);
		sendClientMessage(user, RPL_NOTOPIC(channel.getChannelName()));
		sendClientMessage(user, RPL_NAMREPLY(channel.getChannelName(), channel.getuserList()));
		sendClientMessage(user, RPL_ENDOFNAMES(channel.getChannelName()));
	}

}

Channel		Server::findChannel(std::string channelName) {
	std::vector<Channel>::iterator	iterChannel;

	for (iterChannel = _channelList.begin(); iterChannel < _channelList.end(); iterChannel++) {
		if ((*iterChannel).getChannelName().compare(channelName) == 0) {
			return *iterChannel;
		}
	}
	throw Error::AuthorizeException(); // 채널이 없다는 에러 메세지로 바꾸기 
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
void		Server::commandTOPIC(User* user, std::vector<std::string>& parameters) {
	if (parameters.size() < 3) throw Error::AuthorizeException(); // TODO: 다른 오류(파라미터 부족)로 교체
	if (!isChannel(parameters[1])) throw Error::AuthorizeException(); // TODO: 존재하지 않는 채널이라는 에러로 교체

	std::string	topic;
	std::vector<std::string>::iterator	iter;
	for (iter = parameters.begin() + 2; iter < parameters.end(); iter++) {
		topic = topic + *iter;
	}

	std::vector<Channel>::iterator	iterChannel;
	for (iterChannel = _channelList.begin(); iterChannel < _channelList.end(); iterChannel++) {
		if ((*iterChannel).getChannelName().compare(parameters[1]) == 0) {
			if ((*iterChannel).isOperator(*user))
				(*iterChannel).setTopic(topic);
			else
				throw Error::AuthorizeException(); // TODO: 관리자가 아니라는 에러 메세지 출력으로 바꾸기 
		}
	}
	
}

// void	Server::commandNAMES(User* user, std::vector<std::string>& parameters) {
	
// }

// void	Server::commandUser(User* user, std::vector<std::string>& parameters) {


// 	sendClientMessage(user,
// 		":127.0.0.1 001 " + user->getNickname() + " :\033[1;32mWelcome to the " + SERVER_NAME + "\e[0m " + \
// 		user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname()
// 	);
// }