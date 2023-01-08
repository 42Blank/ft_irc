/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasong <jasong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 04:09:31 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/08 13:42:50 by jasong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

void	Server::commandCAP(User* user, stringVector& parameters) {
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_CAP));
	if (!parameters[1].compare("LS")) sendMessage(user, "CAP * LS");
}

void	Server::commandPING(User* user, stringVector& parameters) {
	if (user->getIsVerified() != ALL_VERIFIED) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NOORIGIN));
	else if (parameters.size() > 2) throw std::runtime_error(Error(ERR_NOSUCHSERVER, parameters[1]));

	sendMessage(user, "PONG 127.0.0.1 :" + parameters[1]);
}

void	Server::commandPONG(User* user, stringVector& parameters) {
	if (user->getIsVerified() != ALL_VERIFIED) throw std::runtime_error(Error(ERR_NOTREGISTERED));
	if (parameters.size() < 2) throw std::runtime_error(Error(ERR_NOORIGIN));
	else if (parameters.size() > 2) throw std::runtime_error(Error(ERR_NOSUCHSERVER, parameters[1]));
}

void	Server::commandQUIT(User* user, stringVector& parameters) {
	std::string		quitUserHostname = user->getHostname();
	std::string		quitUserNickname = user->getNickname();
	std::string		quitMsg = ft_getMsgString(parameters, 0);
	stringVector&	channelList = user->getChannelList();

	sendMessage(user, ErrorReply(":Closing Link:", quitUserHostname, "(Quit: " + quitMsg + ")"));
	for (stringIter iter = channelList.begin(); iter < channelList.end(); iter++) {
		if (!isChannel(*iter)) continue;
		Channel* ch = findChannel(*iter);
		sendMessageBroadcast(1, ch, user, "QUIT :" + quitMsg);
		if (ch->isOperator(quitUserNickname)) {
			if (ch->deleteOperatorUser(user->getNickname())) {	// 오퍼레이터가 새로 바뀌었다.
				sendMessageBroadcast(1, ch, user, "MODE " + ch->getChannelName() + " +o :" + (*ch->getOperatorVector().begin())->getNickname());
			} else {
				ch->setIsDeleted(true);
			}
		}
		else ch->deleteNormalUser(quitUserNickname);
	}
	user->setIsDisconnected(true);
}

void	Server::commandWHO(User* user, stringVector& parameters) {
	if (parameters.size() != 2) throw std::runtime_error(Error(ERR_NEEDMOREPARAMS, CMD_WHO));

	try {
		User*	userFound = findUser(parameters[1]);
		sendMessage(user, Reply(RPL_WHOREPLY, user->getNickname(),
			"* ~" + userFound->getUsername() + " " + userFound->getHostname() + " 127.0.0.1 " + parameters[1] + " H :0 " + userFound->getUsername()));
		sendMessage(user, Reply(RPL_ENDOFWHO, user->getNickname() + " " + parameters[1]));
	} catch (std::exception e) {
		throw std::runtime_error(Error(ERR_NOSUCHSERVER, parameters[1]));
	}
}
