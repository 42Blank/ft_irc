/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 03:50:34 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/04 15:45:23 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"

User::User(void) {
	_clientAddress = new sockaddr_in();
	_clientAddressSize = new socklen_t(sizeof(_clientAddress));
	_isVerified = 0b000;
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

	setSocketFD(instance.getSocketFD());
	_clientAddress = new sockaddr_in(*instance.getAddressPtr());
	_clientAddressSize = new socklen_t(*instance.getAddressSizePtr());
	_nickname = instance.getNickname();
	_username = instance.getUsername();
	_hostname = instance.getHostname();
	return *this;
}

std::vector<struct pollfd>::iterator	User::getSocketFD() const {
	return _clientFDIterator;
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

std::string	User::getHostname() const {
	return _hostname;
}

short	User::getIsVerified() const {
	return _isVerified;
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

void	User::setSocketFD(std::vector<struct pollfd>::iterator fdIter) {
	_clientFDIterator = fdIter;
}

void	User::setIsVerified(short what) {
	_isVerified |= what;
}

std::ostream& operator<<(std::ostream& out, const User& instance) {
	out << "User: [" << instance.getNickname() << "] (" << instance.getUsername() << ")";
	return out;
}
