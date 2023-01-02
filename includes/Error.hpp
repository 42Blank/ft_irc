/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:30:34 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/03 00:16:31 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# include "irc.hpp"
# include <string>

# define ERR_NONICKNAMEGIVEN	"431" // No nickname given
# define ERR_ERRONEUSNICKNAME	"432" // Erroneous nickname
# define ERR_NICKNAMEINUSE		"433" // Nickname is already in use
# define ERR_USERNOTINCHANNEL	"441" // You're not on that channel
# define ERR_NOTONCHANNEL		"442" // They aren't on that channel
# define ERR_NOTREGISTERED		"451" // You have not registered
# define ERR_NEEDMOREPARAMS		"461" // Not enough parameters"
# define ERR_PASSWDMISMATCH		"464" // Password incorrect

class Error: public std::exception {
	private:
		std::string		_errCode;
		std::string		_param1;
		std::string		_param2;

		std::string		getErrorMessage(std::string code) const;
	public:
		Error(std::string errCode);
		Error(std::string errCode, std::string param1);
		Error(std::string errCode, std::string param1, std::string param2);
		const char* what() const throw();
};

#endif
