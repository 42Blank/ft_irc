/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 03:41:37 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/07 02:02:05 by jiychoi          ###   ########.fr       */
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
			if (str[i] == NICK_AVAILABLE_CHAR[j]) {
				isValidChar = true;
				break;
			}
		}
		if (isValidChar) continue;
		if (i == 0 && isalpha(str[i])) continue;
		else if (isalnum(str[i])) continue;
		return false;
	}
	return true;
}

std::string ft_getStringAfterColon(std::vector<std::string> parameter) {
	std::vector<std::string>::iterator iter;
	std::string returnStr = "";
	std::string	ret;
	bool flag = false;

	for (iter = parameter.begin(); iter < parameter.end(); iter++)
	{
		if ((*iter)[0] == ':')
		{
			flag = true;
			returnStr += (*iter);
			continue;
		}
		if (flag)
			returnStr += " " + *iter;
	}
	try {
		ret = returnStr.substr(1, returnStr.length() - 1);
		return (ret);
	}
	catch (std::exception &e) {
		return ("");
	}
}
