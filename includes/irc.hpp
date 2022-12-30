/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 13:44:22 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/30 04:48:23 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <cstring>
# include <string>
# include <vector>
# include <sstream>
# include <cstdlib>
# include <unistd.h>
# include <exception>

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <arpa/inet.h>
# include <netinet/in.h>

std::vector<std::string>	ft_split(std::string input, char delimiter);
void						ft_replaceStr(std::string& str, std::string src, std::string des);

#endif
