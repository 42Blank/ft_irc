/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 19:31:32 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/31 16:19:14 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Error.hpp"

const char* Error::SocketOpenException::what() const throw() {
	return "Socket Open Failed";
}

const char*	Error::ParseMessageException::what() const throw() {
	return "No Nickname Sent";
}

const char*	Error::AuthorizeException::what() const throw() {
	return "Authorization Failed";
}

const char*	Error::SendMessageException::what() const throw() {
	return "Failed to send message to client";
}
