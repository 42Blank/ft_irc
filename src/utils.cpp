/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 03:41:37 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/30 03:50:45 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/irc.hpp"

std::vector<std::string>	ft_split(std::string input, char delimiter) {
	std::vector<std::string> returnVector;
	std::stringstream stream(input);
	std::string temp;

	while (std::getline(stream, temp, delimiter)) returnVector.push_back(temp);

	return returnVector;
}
