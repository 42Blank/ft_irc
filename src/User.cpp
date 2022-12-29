/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 03:50:34 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/30 03:50:35 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"

User::User() {
	std::cout << "user created\n";
	_client_address = new sockaddr_in();
	_client_address_size = sizeof(_client_address);
}

User::~User() {
	std::cout << "user deleted\n";
	delete _client_address;
}

int					User::getSocketDesc() {
	return _client_socket;
}

struct sockaddr_in*	User::getAddressPtr() {
	return _client_address;
}

socklen_t*			User::getAddressSizePtr() {
	return &_client_address_size;
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

void				User::setSocketDesc(int client_socket) {
	_client_socket = client_socket;
}
