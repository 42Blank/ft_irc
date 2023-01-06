/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandAuth.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:49:06 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/07 03:26:03 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::commandPASS(User& user, stringVector& parameters) {
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_PASS));
	if (_password != parameters[1]) throw std::runtime_error(Error(ERR_PASSWDMISMATCH));

	user.setIsVerified(PASS_VERIFIED);
}

void	Server::commandNICK(User& user, stringVector& parameters) {
	userIter	iter;

	if (!(user.getIsVerified() & PASS_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_NICK));
	if (parameters[1].length() <= 0 || parameters[1].length() > 9 || !ft_isValidNickname(parameters[1]))
		throw std::runtime_error(Error(ERR_ERRONEUSNICKNAME, parameters[1]));

	for (iter = _serverUser.begin(); iter < _serverUser.end(); iter++) {
		if (!(*iter).getNickname().compare(parameters[1]))
			throw std::runtime_error(Error(ERR_NICKNAMEINUSE, parameters[1]));
	}
	user.setNickname(parameters[1]);
	user.setIsVerified(NICK_VERIFIED);
}

void	Server::commandUSER(User& user, stringVector& parameters) {
	if (!(user.getIsVerified() & PASS_VERIFIED))
		throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 5 || parameters[1].length() <= 0)
		throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_NICK));

	user.setUsername(parameters[1]);
	user.setHostname(parameters[3]);
	user.setIsVerified(USER_VERIFIED);
	checkIsVerified(user);
}

void	Server::checkIsVerified(User& user) {
	if (user.getIsVerified() != ALL_VERIFIED) return;

	sendMessage(user,
		"001 " + user.getNickname() + " :\033[1;32mWelcome to the " + SERVER_NAME + "\e[0m " + \
		user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname()
	);
	sendMessage(user,
		"002 " + user.getNickname() + " :\033[1;32mYour host is " + SERVER_NAME + ", " + "running version 0.1\e[0m"
	);
	sendMessage(user,
		"003 " + user.getNickname() + " :\033[1;32mThis server was created at 2022.12.27\e[0m"
	);
	sendMessage(user,
		"004 " + user.getNickname() + " :\033[1;32m" + SERVER_NAME + " 0.1\e[0m"
	);
}
