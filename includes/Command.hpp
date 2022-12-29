/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 03:59:21 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/30 04:32:39 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "irc.hpp"

# define CMD_NICK "NICK"
# define CMD_USER "USER"

void	commandNICK(std::vector<std::string>& parameters);
void	commandUser(std::vector<std::string>& parameters);

#endif
