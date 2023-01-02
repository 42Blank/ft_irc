/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:31:32 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/03 00:19:09 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Error.hpp"

Error::Error(std::string errCode):
	_errCode(errCode), _param1(""), _param2("") {}

Error::Error(std::string errCode, std::string param1):
	_errCode(errCode), _param1(param1), _param2("") {}

Error::Error(std::string errCode, std::string param1, std::string param2):
	_errCode(errCode), _param1(param1), _param2(param2) {}

const char* Error::what() const throw() {
	if (_param1.length() == 0)
		return (_errCode + getErrorMessage(_errCode)).c_str();
	if (_param2.length() == 0)
		return (_errCode + " " + _param1 + getErrorMessage(_errCode)).c_str();
	return (_errCode + " " + _param1 + " " + _param2 + getErrorMessage(_errCode)).c_str();
}

std::string	Error::getErrorMessage(std::string code) const {
	if (code == ERR_NEEDMOREPARAMS) return " :Not enough parameters";
	if (code == ERR_PASSWDMISMATCH) return " :Password incorrect";
}
