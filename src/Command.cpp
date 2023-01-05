/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasong <jasong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 04:09:31 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/06 07:20:06 by jasong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::commandCAP(User& user, std::vector<std::string>& parameters) {
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_CAP));
	if (!parameters[1].compare("LS")) {
		sendMessage(user, "CAP * LS");
	}
}

void	Server::commandPING(User& user, std::vector<std::string>& parameters) {
	if (!(user.getIsVerified() != ALL_VERIFIED)) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NOORIGIN));
	else if (parameters.size() > 2) throw std::runtime_error(Error(ERR_NOSUCHSERVER, parameters[1]));
	sendMessage(user, "PONG 127.0.0.1 :" + parameters[1]);
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
	std::vector<std::string> channelList = user.getChannelList();

	sendMessage(user, ErrorReply(":Closing Link:", quitUserHostname, "(Quit: " + quitMsg + ")"));
	for (std::vector<std::string>::iterator iter = channelList.begin(); iter < channelList.end(); iter++) {
		if (!isChannel(*iter)) continue;
		Channel &ch = findChannel(*iter);
		sendMessageBroadcast(0, ch, user, "QUIT :" + quitMsg);
		if (ch.isOperator(quitUserNickname)) ch.deleteOperatorUser(quitUserNickname);
		else ch.deleteNormalUser(quitUserNickname);
	}
	user.setIsDisconnected(true);
}
