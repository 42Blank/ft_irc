/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 04:09:31 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/04 16:51:33 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::commandCAP(User& user, std::vector<std::string>& parameters) {
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_CAP));
	if (parameters[1] == "LS") {
		sendClientMessage(user, "CAP * LS");
	}
}

void	Server::commandPING(User& user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NOORIGIN));
	else if (parameters.size() > 2) throw std::runtime_error(Error(ERR_NOSUCHSERVER, parameters[1]));
	sendClientMessage(user, "PONG 127.0.0.1 :" + parameters[1]);
}

void	Server::commandPONG(User& user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NOORIGIN));
	else if (parameters.size() > 2) throw std::runtime_error(Error(ERR_NOSUCHSERVER, parameters[1]));
}

void	Server::commandQUIT(User& user, std::vector<std::string>& parameters) {
	std::string quitUserHostname = user.getHostname();
	std::string quitUserNickname = user.getNickname();
	std::string	quitMsg = ft_getStringAfterColon(parameters);
	sendClientMessage2(user, ErrorReply(":Closing Link:", quitUserHostname, "(Quit: " + quitUserNickname + ")"));
	user.setIsDisconnected(true);
}
