/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSendMessage.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasong <jasong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:23:22 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/06 01:05:09 by jasong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::sendMessage(User& user, std::string str) {
	int	fd = user.getSocketFd();
	std::string strToSend = ":" + user.getNickname() + "!" + user.getNickname()  + "@127.0.0.1 " + str + "\r\n";

	if (send(fd, (strToSend).c_str(), strToSend.length(), 0) < 0)
		throw std::runtime_error(Error(ERR_MESSAGESENDFAILED));
}

void	Server::sendMessage(User& sender, User& receiver, std::string str) {
	int fd = receiver.getSocketFd();
	std::string strToSend = ":" + sender.getNickname() + "!" + sender.getNickname()  + "@127.0.0.1 " + str + "\r\n";

	if (send(fd, (strToSend).c_str(), strToSend.length(), 0) < 0)
		throw std::runtime_error(Error(ERR_MESSAGESENDFAILED));
}

// 이건 채널에서 전체 메세지를 보내거나 채널에서 어떤 행동을 했을 때 사용하는 것 -> 채널의 userList가 있어야 한다.
void	Server::sendMessageBroadcast(int mode, Channel& ch, User& sender, std::string str) {
	std::vector<User>::iterator it;
	std::vector<User> operUsers = ch.getOperatorVector();
	std::vector<User> normUsers = ch.getNormalUserVector();

	for (it = operUsers.begin(); it < operUsers.end(); it++) {
		if (mode == 0 || (*it).getNickname().compare(sender.getNickname()))
			sendMessage(sender, *it, str);
	}
	for (it = normUsers.begin(); it < normUsers.end(); it++) {
		if (mode == 0 || (*it).getNickname().compare(sender.getNickname()))
			sendMessage(sender, *it, str);
	}
}
