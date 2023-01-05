/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:31:32 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/05 16:51:02 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Reply.hpp"

static std::string	getReplyMessage(std::string code) {
	if (!code.compare(RPL_NOTOPIC)) return " :No topic is set";
	if (!code.compare(RPL_ENDOFNAMES)) return " :End of /NAMES list";
	return "";
}

std::string	Reply(std::string repCode) {
	return repCode + getReplyMessage(repCode);
}

std::string	Reply(std::string repCode, std::string param1) {
	return repCode + " " + param1 + getReplyMessage(repCode);
}

std::string	Reply(std::string repCode, std::string param1, std::string param2) {
	return repCode + " " + param1 + " " + param2 + getReplyMessage(repCode);
}

static std::string	getErrorMessage(std::string code) {
	if (!code.compare(ERR_NOSUCHNICK)) return " :No such nick/channel";
	if (!code.compare(ERR_NOSUCHSERVER)) return " :No such server";
	if (!code.compare(ERR_NOSUCHCHANNEL)) return " :No such channel";
	if (!code.compare(ERR_NOORIGIN)) return " :No origin specified";
	if (!code.compare(ERR_UNKNOWNCOMMAND)) return " :Unknown command";
	if (!code.compare(ERR_NONICKNAMEGIVEN)) return " :No nickname given";
	if (!code.compare(ERR_ERRONEUSNICKNAME)) return " :Erroneous nickname";
	if (!code.compare(ERR_NICKNAMEINUSE)) return " :Nickname is already in use";
	if (!code.compare(ERR_USERNOTINCHANNEL)) return " :They aren't on that channel";
	if (!code.compare(ERR_NOTONCHANNEL)) return " :You're not on that channel";
	if (!code.compare(ERR_NOTREGISTERED)) return " :You have not registered";
	if (!code.compare(ERR_NEEDMOREPARAMS)) return " :Not enough parameters";
	if (!code.compare(ERR_PASSWDMISMATCH)) return " :Password incorrect";
	if (!code.compare(ERR_CHANOPRIVSNEEDED)) return " :You're not channel operator";

	if (!code.compare(ERR_USERSDONTMATCH)) return " :Can't view modes for other users";
	if (!code.compare(ERR_SERVEROPENFAILED)) return " :Failed to open server";
	if (!code.compare(ERR_CLIENTCONNECTFAILED)) return " :Failed to connect with client";
	if (!code.compare(ERR_CANNOTFINDUSERFD)) return " :Cannot find user by fd";
	if (!code.compare(ERR_MESSAGESENDFAILED)) return " :Failed to send message";
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

std::string	ErrorReply(std::string param1, std::string param2, std::string param3) {
	return "ERROR " + param1 + " " + param2 + " " + param3;
}
