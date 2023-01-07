/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 16:30:17 by jiychoi           #+#    #+#             */
/*   Updated: 2023/01/08 02:10:29 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/Channel.hpp"

bool	Channel::addBadWord(std::string word) {
	stringIter	iter = std::find(_badWordList.begin(), _badWordList.end(), word);
	if (iter == _badWordList.end()) {
		_badWordList.push_back(word);
		return true;
	}
	return false;
}

bool	Channel::removeBadWord(std::string word) {
	stringIter	iter = std::find(_badWordList.begin(), _badWordList.end(), word);
	if (iter != _badWordList.end()) {
		_badWordList.erase(iter);
		return true;
	}
	return false;
}

std::string	Channel::parseBotCommand(std::string& message) {
	stringVector parameters = ft_split(message, ' ');

	if (!parameters[0].compare(BOT_CMD_42WORLD)) return command42WORLD();
	if (!parameters[0].compare(BOT_CMD_HELP)) return commandHELP();
	if (!parameters[0].compare(BOT_CMD_TIME)) return commandTIME();
	if (!parameters[0].compare(BOT_CMD_ADD)) return commandADD(parameters);
	if (!parameters[0].compare(BOT_CMD_REMOVE)) return commandREMOVE(parameters);
	return "";
}

std::string	Channel::command42WORLD() {
	return "<bot> 42월드에 놀러오세요 http://42world.kr";
}

std::string	Channel::commandHELP() {
	return "[" + std::string(BOT_NAME) + "] 사용 가능한 명령어 목록입니다.\n" +
		"\t{ " + BOT_CMD_HELP + " }" + "봇 도움말을 출력해요.\n" +
		"\t{ " + BOT_CMD_TIME + " }" + "현재 시간을 알려줘요.\n" +
		"\t{ " + BOT_CMD_ADD + "...<추가할 단어> }" + "금지어를 추가할 수 있어요. (관리자 권한 필요)\n" +
		"\t{ " + BOT_CMD_REMOVE + "...<삭제할 단어> }" + "금지어를 삭제할 수 있어요. (관리자 권한 필요)";
}

std::string Channel::commandADD(stringVector& parameters) {
	if (parameters.size() == 1) return "<bot> 추가할 단어가 없는 것 같아요...";

	std::string	message = "추가된 단어: ";
	stringIter	iter;
	for (iter = parameters.begin() + 1; iter < parameters.end(); iter++)
		if (addBadWord(*iter)) message += *iter + " ";
	return message;
}

std::string Channel::commandREMOVE(stringVector& parameters) {
	if (parameters.size() == 1) return "<bot> 삭제할 단어가 없는 것 같아요...";
	if (parameters.size() > 2) return "<bot> 한 번에 한 개의 단어만 삭제할 수 있어요";

	std::string	message = "삭제한 단어: ";
	if (removeBadWord(parameters[1])) message += parameters[1];
	return message;
}

std::string Channel::commandTIME() {
	time_t	currentTime = time(NULL);
	return	"현재 서버시간은 [" + std::string(ctime(&currentTime)) + "] 입니다";
}
