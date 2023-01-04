/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:30:34 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/04 12:51:57 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
# define REPLY_HPP

# include "irc.hpp"
# include <string>
# include <stdexcept>

// Reply

# define RPL_WELCOME				"001"
# define RPL_YOURHOST				"002"
# define RPL_CREATED				"003"
# define RPL_MYINFO					"004"

# define RPL_NOTOPIC				"331" // No topic is set
# define RPL_TOPIC					"332"
# define RPL_NAMREPLY				"353"
# define RPL_ENDOFNAMES				"366" // End of NAMES list

// Error

# define ERR_NOSUCHSERVER			"402" // No such server
# define ERR_NOSUCHCHANNEL			"403" // No such channel
# define ERR_NOORIGIN				"409" // No origin specified
# define ERR_UNKNOWNCOMMAND			"421" // Unknown command
# define ERR_NONICKNAMEGIVEN		"431" // No nickname given
# define ERR_ERRONEUSNICKNAME		"432" // Erroneous nickname
# define ERR_NICKNAMEINUSE			"433" // Nickname is already in use
# define ERR_USERNOTINCHANNEL		"441" // You're not on that channel
# define ERR_NOTONCHANNEL			"442" // They aren't on that channel
# define ERR_NOTREGISTERED			"451" // You have not registered
# define ERR_NEEDMOREPARAMS			"461" // Not enough parameters
# define ERR_PASSWDMISMATCH			"464" // Password incorrect
# define ERR_CHANOPRIVSNEEDED		"482" // You're not channel operator

# define ERR_SERVEROPENFAILED		"600" // Failed to open server
# define ERR_CLIENTCONNECTFAILED	"601" // Failed to connect with client
# define ERR_CANNOTFINDUSERFD		"602" // Cannot find user by fd
# define ERR_MESSAGESENDFAILED		"603" // Failed to send message

std::string	Error(std::string errCode);
std::string	Error(std::string errCode, std::string param1);
std::string	Error(std::string errCode, std::string param1, std::string param2);

std::string Reply(std::string repCode);
std::string Reply(std::string repCode, std::string param1);
std::string Reply(std::string repCode, std::string param1, std::string param2);

std::string	ErrorReply(std::string param1, std::string param2, std::string param3);

# endif
