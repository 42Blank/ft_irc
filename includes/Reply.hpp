#ifndef REPLY_HPP
# define REPLY_HPP

#include <string>

// 331
std::string	RPL_NOTOPIC(std::string channelName);
// 332
std::string	RPL_TOPIC(std::string channelName, std::string topic);
// 353
std::string	RPL_NAMREPLY(std::string channelName, std::string userList);

std::string	RPL_ENDOFNAMES(std::string channelName);


# endif