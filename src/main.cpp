/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 13:45:58 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/28 13:46:23 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <iostream>

int main(int ac, char **av) {
	std::cout << "hello world!\n";


	if (ac != 3)
	{
		printf("Usage : %s <port> <password>\n", av[0]);
		exit(1);	
	}

	Server server =  Server(av[1]);
	server.serverOn();
}
