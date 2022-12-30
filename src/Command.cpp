/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 04:09:31 by jiychoi           #+#    #+#             */
/*   Updated: 2022/12/30 05:09:02 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Command.hpp"

void	commandNICK(User* user, std::vector<std::string>& parameters) {
	user->setNickname(*(parameters.end() - 1));

	// 닉네임 검증이 들어갈 부분
	// 통과 실패할 경우 예외 throw 해주세요 (Error::AuthorizeException())
}

void	commandUser(User* user, std::vector<std::string>& parameters, std::vector<User*>& user_vector) {
	user->setUsername(*(parameters.end() - 1));

	// 유저명 검증이 들어갈 부분
	user_vector.push_back(user);
	// 통과했을 때만 _user_vector에 push_back
	// 닉네임 검증은 어차피 위에서 해주기 때문에 commandUser에서만 push_back 하면 됨

	std::vector<User*>::iterator	userIter;
	for (userIter = user_vector.begin(); userIter != user_vector.end(); userIter++) {
		std::cout << "User [" << (*userIter)->getUsername() << "] (" << (*userIter)->getNickname() << ") joined\n";
	}
	// 위 코드 주석해제하면 유저 목록 볼 수 있음
}
