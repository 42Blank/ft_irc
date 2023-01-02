/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:31:32 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/03 02:08:24 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Reply.hpp"

static std::string	getErrorMessage(std::string code) {
	if (code == ERR_NOSUCHCHANNEL) return " :No such channel";
	if (code == ERR_UNKNOWNCOMMAND) return " :Unknown command";
	if (code == ERR_NONICKNAMEGIVEN) return " :No nickname given";
	if (code == ERR_ERRONEUSNICKNAME) return " :Erroneous nickname";
	if (code == ERR_NICKNAMEINUSE) return " :Nickname is already in use";
	if (code == ERR_USERNOTINCHANNEL) return " :You're not on that channel";
	if (code == ERR_NOTONCHANNEL) return " :They aren't on that channel";
	if (code == ERR_NOTREGISTERED) return " :You have not registered";
	if (code == ERR_NEEDMOREPARAMS) return " :Not enough parameters";
	if (code == ERR_PASSWDMISMATCH) return " :Password incorrect";
	if (code == ERR_CHANOPRIVSNEEDED) return " :You're not channel operator";

	if (code == ERR_SERVEROPENFAILED) return " :Failed to open server";
	if (code == ERR_CLIENTCONNECTFAILED) return " :Failed to connect with client";
	if (code == ERR_CANNOTFINDUSERFD) return " :Cannot find user by fd";
	if (code == ERR_MESSAGESENDFAILED) return " :Failed to send message";
	return " :Unknown error";
}

std::string	Error(std::string errCode) {
	return errCode + getErrorMessage(errCode);
}

std::string	Error(std::string errCode, std::string param1) {
	return errCode + " " + param1 + getErrorMessage(errCode);
}

std::string	Error(std::string errCode, std::string param1, std::string param2) {
	return errCode + " " + param1 + " " + param2 + getErrorMessage(errCode);
}
