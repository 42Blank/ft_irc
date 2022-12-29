/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 04:09:31 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/30 04:18:40 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

void	commandNICK(std::vector<std::string>& parameters) {
	std::vector<std::string>::iterator	paramIter;
	for (paramIter = parameters.begin(); paramIter != parameters.end(); paramIter++) {
		std::cout << "Nickname: [" << *paramIter << "]\n";
	}
}

void	commandUser(std::vector<std::string>& parameters) {
	std::vector<std::string>::iterator	paramIter;
	for (paramIter = parameters.begin(); paramIter != parameters.end(); paramIter++) {
		std::cout << "Username: [" << *paramIter << "]\n";
	}
}
