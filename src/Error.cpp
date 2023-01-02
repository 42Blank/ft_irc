/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:31:32 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/03 01:06:09 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Error.hpp"

Error::Error(std::string errCode):
	_errCode(errCode), _param1(""), _param2("") {}

Error::Error(std::string errCode, std::string param1):
	_errCode(errCode), _param1(param1), _param2("") {}

Error::Error(std::string errCode, std::string param1, std::string param2):
	_errCode(errCode), _param1(param1), _param2(param2) {}

Error::~Error() throw() {}

const char* Error::what() const throw() {
	if (_param1.length() == 0)
		return (_errCode + getErrorMessage(_errCode)).c_str();
	if (_param2.length() == 0)
		return (_errCode + " " + _param1 + getErrorMessage(_errCode)).c_str();
	return (_errCode + " " + _param1 + " " + _param2 + getErrorMessage(_errCode)).c_str();
}

std::string	Error::getErrorMessage(std::string code) const {
	if (code == ERR_UNKNOWNCOMMAND) return " :Unknown command";
	if (code == ERR_NONICKNAMEGIVEN) return " :No nickname given";
	if (code == ERR_ERRONEUSNICKNAME) return " :Erroneous nickname";
	if (code == ERR_NICKNAMEINUSE) return " :Nickname is already in use";
	if (code == ERR_USERNOTINCHANNEL) return " :You're not on that channel";
	if (code == ERR_NOTONCHANNEL) return " :They aren't on that channel";
	if (code == ERR_NOTREGISTERED) return " :You have not registered";
	if (code == ERR_NEEDMOREPARAMS) return " :Not enough parameters";
	if (code == ERR_PASSWDMISMATCH) return " :Password incorrect";

	if (code == ERR_SERVEROPENFAILED) return " :Failed to open server";
	if (code == ERR_CLIENTCONNECTFAILED) return " :Failed to connect with client";
	if (code == ERR_CANNOTFINDUSERFD) return " :Cannot find user by fd";
	if (code == ERR_MESSAGESENDFAILED) return " :Failed to send message";
}
