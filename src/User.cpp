/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 03:50:34 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/30 23:11:31 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"

User::User() {
	std::cout << "user created\n";
	_clientAddress = new sockaddr_in();
	_clientAddressSize = sizeof(_clientAddress);
}

User::~User() {
	std::cout << "user deleted\n";
	delete _clientAddress;
}

int					User::getSocketDesc() {
	return _clientSocket;
}

struct sockaddr_in*	User::getAddressPtr() {
	return _clientAddress;
}

socklen_t*			User::getAddressSizePtr() {
	return &_clientAddressSize;
}

std::string			User::getNickname() {
	return _nickname;
}

std::string			User::getUsername() {
	return _username;
}

void				User::setNickname(std::string nickname) {
	_nickname = nickname;
}

void				User::setUsername(std::string username) {
	_username = username;
}

void				User::setSocketDesc(int clientSocket) {
	_clientSocket = clientSocket;
}
