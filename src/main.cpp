/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 13:45:58 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/07 02:42:23 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Server.hpp"

int main(int argc, char* argv[]) {
	if (argc != 3)
	{
		printf("Usage : %s <port> <password>\n", argv[0]);
		return (1);
	}
	std::cout << "oo                                                       \n                                                         \ndP 88d888b. .d8888b. .d8888b. .d8888b. 88d888b. dP   .dP \n88 88'  `88 88'  `'' Y8ooooo. 88ooood8 88'  `88 88   d8' \n88 88       88.  ...       88 88.  ... 88       88 .88'  \ndP dP       `88888P' `88888P' `88888P' dP       8888P'   \n                                                       \n";

	try {
		Server server = Server(argv[1], argv[2]);
		server.serverOn();
	} catch (std::exception& e) {
		std::cout << "ircserv: Error: " << e.what() << "\n";
		return (1);
	}
}
