#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"

# include <string>
# include <vector>

class Channel {
	private:
		std::string		_channelName;
		User			_operator;	// user의 nickname 저장
		std::string		_topic;
		std::vector<User>	_c_userList;

	public:
		Channel(User &user, std::string name);
		~Channel();

		void				joinNewUser(User user);
		bool				isOperator(User user);
		bool				isUser(std::string nickname);

		std::string			getTopic();
		std::string			getUserList();
		std::string			getChannelName();

		void				setTopic(std::string topic);

		void				deleteNormalUser(std::string nickname);
		void				deleteOperatorUser(std::string nickname);

};


#endif
