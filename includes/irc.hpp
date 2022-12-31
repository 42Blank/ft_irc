/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 13:44:22 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/31 17:25:39 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <cstring>
# include <cctype>
# include <ctime>
# include <cstdlib>
# include <string>
# include <vector>
# include <sstream>
# include <unistd.h>
# include <exception>

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <netinet/in.h>

# define NICK_AVAILABLE_CHAR "[]\\`_^{}|"

std::vector<std::string>	ft_split(std::string input, char delimiter);
void						ft_replaceStr(std::string& str, std::string src, std::string des);
bool						ft_isValidNickname(const std::string& str);

#endif
