/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Verify.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasong <jasong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:46:02 by jasong            #+#    #+#             */
/*   Updated: 2022/12/30 20:02:07 by jasong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

bool		Server::isValidNickname(const std::string& nickname) {
	std::vector<User *>::iterator	iter;

	for (iter = _user_vector.begin(); iter < _user_vector.end(); iter++) {
		if ((*iter)->getNickname() == nickname)
			throw Error::AuthorizeException();
	}
	return (true);
}

bool		Server::isValidUsername(const std::string& username) {
	std::vector<User *>::iterator	iter;

	for (iter = _user_vector.begin(); iter < _user_vector.end(); iter++) {
		if ((*iter)->getNickname() == username)
			throw Error::AuthorizeException();
	}
	return (true);
}

void		Server::testUser(void) {
	std::vector<User *>::iterator	iter;
	
	for (iter = _user_vector.begin(); iter < _user_vector.end(); iter++) {
		std::cout << (*iter)->getNickname() << '\n';
	}
}