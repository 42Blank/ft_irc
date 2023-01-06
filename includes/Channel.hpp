/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 10:08:13 by san              #+#    #+#             */
/*   Updated: 2023/01/07 01:59:13 by jiychoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"

# include <string>
# include <vector>

class Channel {
	private:
		std::string			_channelName;
		std::vector<User>	_operator;	// user의 nickname 저장
		std::string			_topic;
		std::vector<User>	_c_userList;
		std::string			_channelMode;

	public:
		Channel(User &user, std::string name);
		~Channel();

		void				joinNewUser(User user);
		bool				isOperator(std::string nickname);
		bool				isUser(std::string nickname);

		std::string			getTopic();
		std::string			getUserList();
		std::vector<User>	getOperatorVector();
		std::vector<User>	getNormalUserVector();

		std::string			getChannelName();
		std::string			getChannelMode();

		void				setTopic(std::string topic);
		void				setChannelMode(std::string channelMode);

		void				deleteNormalUser(std::string nickname);
		int					deleteOperatorUser(std::string nickname);

};


#endif
