/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandAuth.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:49:06 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/06 10:02:27 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::commandPASS(User& user, std::vector<std::string>& parameters) {
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_PASS));
	if (_password != parameters[1]) throw std::runtime_error(Error(ERR_PASSWDMISMATCH));

	user.setIsVerified(PASS_VERIFIED);
}

void	Server::commandNICK(User& user, std::vector<std::string>& parameters) {
	std::vector<User>::iterator	iter;

	const std::string nickname = parameters[1];
	if (!(user.getIsVerified() & PASS_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_NICK));
	if (nickname.length() <= 0 || nickname.length() > 9) throw std::runtime_error(Error(ERR_ERRONEUSNICKNAME, nickname));
	if (!ft_isValidNickname(nickname)) throw std::runtime_error(Error(ERR_ERRONEUSNICKNAME, nickname));

	for (iter = _s_userList.begin(); iter < _s_userList.end(); iter++) {
		if (!(*iter).getNickname().compare(nickname)) throw std::runtime_error(Error(ERR_NICKNAMEINUSE, nickname));
	}
	user.setNickname(nickname);
	
	user.setIsVerified(NICK_VERIFIED);
	//checkIsVerified(user);
}

void	Server::commandUSER(User& user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() & PASS_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 5)
		throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_NICK));
	if (parameters[1].length() <= 0)
		throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_NICK));

	user.setUsername(parameters[1]);
	user.setHostname(parameters[3]);
	user.setIsVerified(USER_VERIFIED);
	checkIsVerified(user);
}

void	Server::checkIsVerified(User& user) {
	// if (user.getIsVerified() != ALL_VERIFIED) return;
	if (!(user.getIsVerified() & ALL_VERIFIED)) return;

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
