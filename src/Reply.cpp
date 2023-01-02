
#include "../includes/Reply.hpp"

#include <string>

// 331
std::string	RPL_NOTOPIC(std::string channelName) { 
//   "<client> <channel> :No topic is set"
	return channelName + " :No topic is set";
}

// 332
std::string	RPL_TOPIC(std::string channelName, std::string topic) {
//   "<client> <channel> :<topic>"
	return channelName + " :" + topic; 
}

// 353
std::string	RPL_NAMREPLY(std::string channelName, std::string userList) {
	return channelName + " :" + userList;
}

std::string	RPL_ENDOFNAMES(std::string channelName) {
//   "<client> <channel> :End of /NAMES list"
	return channelName + " :End of /NAMES list";
}


