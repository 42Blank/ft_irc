/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 03:50:34 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/31 17:33:06 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"

User::User(void) {
	_clientAddress = new sockaddr_in();
	_clientAddressSize = new socklen_t(sizeof(_clientAddress));
}

User::~User(void) {
	std::cout << "deleted user: " << _nickname << "\n";
	delete _clientAddress;
	delete _clientAddressSize;
}

User::User(const User& instance) {
	*this = instance;
}

User&	User::operator=(const User& instance) {
	std::cout << "copied user: " << instance.getNickname() << "\n";

	_clientSocket = instance.getSocketDesc();
	_clientAddress = new sockaddr_in(*instance.getAddressPtr());
	_clientAddressSize = new socklen_t(*instance.getAddressSizePtr());
	_nickname = instance.getNickname();
	_username = instance.getUsername();
	return *this;
}

int	User::getSocketDesc() const {
	return _clientSocket;
}

struct sockaddr_in*	User::getAddressPtr() const {
	return _clientAddress;
}

socklen_t*	User::getAddressSizePtr() const {
	return _clientAddressSize;
}

std::string	User::getNickname() const {
	return _nickname;
}

std::string	User::getUsername() const {
	return _username;
}

std::string User::getHostname() const {
	return _hostname;
}

void	User::setNickname(std::string nickname) {
	_nickname = nickname;
}

void	User::setUsername(std::string username) {
	_username = username;
}

void	User::setHostname(std::string hostname) {
	_hostname = hostname;
}

void	User::setSocketDesc(int clientSocket) {
	_clientSocket = clientSocket;
}

std::ostream& operator<<(std::ostream& out, const User& instance) {
	out << "User: [" << instance.getNickname() << "] (" << instance.getUsername() << ")";
	return out;
}
