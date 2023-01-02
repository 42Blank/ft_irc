/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:31:32 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/03 02:34:38 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Reply.hpp"

static std::string	getReplyMessage(std::string code) {
	if (code == RPL_NOTOPIC) return " :No topic is set";
	if (code == RPL_ENDOFNAMES) return " :End of NAMES list";
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
