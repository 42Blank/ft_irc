/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:30:34 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/01 18:15:14 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# include "irc.hpp"

class Error {
	public:
		class SocketOpenException: public std::exception {
			public:
				const char* what() const throw();
		};
		class ParseMessageException: public std::exception {
			public:
				const char* what() const throw();
		};
		class AuthorizeException: public std::exception {
			public:
				const char* what() const throw();
		};
		class SendMessageException: public std::exception {
			public:
				const char* what() const throw();
		};
		class UserNotFoundException: public std::exception {
			public:
				const char* what() const throw();
		};
		class PasswordException: public std::exception {
			public:
				const char* what() const throw();
		};
};

#endif
