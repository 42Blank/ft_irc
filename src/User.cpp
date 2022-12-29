#include "../includes/User.hpp"

#include <iostream>

User::User() {
	std::cout << "user created\n";
	_client_address = new sockaddr_in();
	_client_address_size = new socklen_t();
}

User::~User() {
	std::cout << "user deleted\n";
	delete _client_address;
	delete _client_address_size;
}

int					User::getSocketDesc() {
	return _client_socket;
}

struct sockaddr_in*	User::getAddressPtr() {
	return _client_address;
}

socklen_t*			User::getAddressSizePtr() {
	return _client_address_size;
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
