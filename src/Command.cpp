/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 04:09:31 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/31 15:07:55 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void	Server::commandNICK(User* user, std::vector<std::string>& parameters) {
	std::vector<User>::iterator	iter;
	const std::string nickname = *(parameters.end() - 1);
	user->setNickname(nickname);

	for (iter = _user_vector.begin(); iter < _user_vector.end(); iter++) {
		if ((*iter).getNickname() == nickname)
			throw Error::AuthorizeException();
	}
}

void	Server::commandUser(User* user, std::vector<std::string>& parameters) {
	std::vector<User>::iterator	iter;
	const std::string username = *(parameters.end() - 1);
	user->setUsername(username);

	for (iter = _user_vector.begin(); iter < _user_vector.end(); iter++) {
		if ((*iter).getUsername() == username)
			throw Error::AuthorizeException();
	}
	_user_vector.push_back(*user);
}
