/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Verify.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 19:46:02 by jasong            #+#    #+#             */
/*   Updated: 2022/12/31 00:42:15 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void		Server::testUser(void) {
	std::vector<User>::iterator	iter;

	for (iter = _user_vector.begin(); iter < _user_vector.end(); iter++) {
		std::cout << (*iter) << '\n';
	}
}
