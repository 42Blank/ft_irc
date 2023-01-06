/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 13:44:22 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/06 10:05:41 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <cstring>
# include <cctype>
# include <ctime>
# include <cstdlib>
# include <string>
# include <vector>
# include <sstream>
# include <unistd.h>
# include <exception>
# include <algorithm>

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/poll.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <netinet/in.h>

# define CMD_CAP "CAP"
# define CMD_PASS "PASS"
# define CMD_NICK "NICK"
# define CMD_USER "USER"
# define CMD_MODE "MODE"
# define CMD_JOIN "JOIN"
# define CMD_TOPIC "TOPIC"
# define CMD_NAMES "NAMES"
# define CMD_MSG "PRIVMSG"
# define CMD_PART "PART"
# define CMD_PING "PING"
# define CMD_PONG "PONG"
# define CMD_QUIT "QUIT"
# define CMD_KICK "KICK"
# define CMD_WHO "WHO"

# define NICK_AVAILABLE_CHAR "[]\\`_^{}|"

std::vector<std::string>	ft_split(std::string input, char delimiter);
void						ft_replaceStr(std::string& str, std::string src, std::string des);
bool						ft_isValidNickname(const std::string& str);
std::string					ft_getStringAfterColon(std::vector<std::string> parameter);
int							ft_checkPollReturnEvent(short revents);
bool						ft_checkIsCommandValid(std::string command, bool isVerified);

#endif
