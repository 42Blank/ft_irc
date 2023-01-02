#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"

# include <string>
# include <vector>

class Channel {
	private:
		std::string		_channelName;
		std::string		_operator;	// user의 nickname 저장
		std::string		_topic;		
		std::vector<User>	_userList;

	public:
		Channel(User &user, std::string name);
		~Channel();

		void				joinNewUser(User user);
		bool				isOperator(User user);

		std::string			getTopic();
		std::string			getuserList();
		std::string			getChannelName();

		void				setTopic(std::string topic);

		void				deleteNormalUser(std::string nickname);
		// void				deleteOperatorUser(std::string nickname);

};


#endif