/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 04:09:31 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/31 00:41:57 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

void	commandNICK(User* user, std::vector<std::string>& parameters, std::vector<User>& user_vector) {
	std::vector<User>::iterator	iter;
	const std::string nickname = *(parameters.end() - 1);
	user->setNickname(nickname);

	for (iter = user_vector.begin(); iter < user_vector.end(); iter++) {
		if ((*iter).getNickname() == nickname)
			throw Error::AuthorizeException();
	}
}

void	commandUser(User* user, std::vector<std::string>& parameters, std::vector<User>& user_vector) {
	std::vector<User>::iterator	iter;
	const std::string username = *(parameters.end() - 1);
	user->setUsername(username);

	for (iter = user_vector.begin(); iter < user_vector.end(); iter++) {
		if ((*iter).getNickname() == username)
			throw Error::AuthorizeException();
	}
	user_vector.push_back(*user);
}
