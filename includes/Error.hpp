/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:30:34 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/30 02:59:36 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

#include <exception>
#include <string>

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
};

#endif
