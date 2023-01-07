/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasong <jasong@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 03:41:37 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/08 05:26:54 by jasong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/irc.hpp"
#include <cctype>

stringVector	ft_split(std::string input, char delimiter) {
	stringVector		returnVector;
	std::stringstream	stream(input);
	std::string			temp;

	while (std::getline(stream, temp, delimiter)) returnVector.push_back(temp);

	return returnVector;
}

void	ft_replaceStr(std::string& str, std::string src, std::string des) {
	std::size_t	pos;

	while ((pos = str.find(src)) != str.npos) {
		str.erase(pos, 1);
		str.insert(pos, des);
	}
}

bool	ft_isValidNickname(const std::string& str) {
	int		length = str.length();
	bool	isValidChar;

	for (int i = 0; i < length; i++) {
		isValidChar = false;
		for (int j = 0; j < 9; j++) {
			if (str[i] != NICK_AVAILABLE_CHAR[j]) continue;
			isValidChar = true;
			break;
		}
		if (isValidChar || (i == 0 && isalpha(str[i])) || isalnum(str[i])) continue;
		return false;
	}
	return true;
}

bool	ft_isValidChannelName(const std::string& str) {
	for (int i = 0; i < 4; i++)
		if (str[0] == CHANNEL_AVAILABLE_CHAR[i]) return true;
	return false;
}

std::string ft_getStringAfterColon(stringVector parameter) {
	stringIter	iter;
	std::string	returnStr = "";
	std::string	ret;
	bool flag = false;

	for (iter = parameter.begin(); iter < parameter.end(); iter++) {
		if ((*iter)[0] == ':') {
			flag = true;
			returnStr += (*iter);
			continue;
		}
		if (flag) returnStr += " " + *iter;
	}
	try {
		ret = returnStr.substr(1, returnStr.length() - 1);
		return (ret);
	}
	catch (std::exception& e) {
		return ("");
	}
}

std::string	ft_toUpperString(const std::string str) {
	std::string	retStr = "";
	int			i = 0;

	while (str[i]) {
		retStr += std::toupper(str[i]);
		i++;
	}
	return (retStr);
}