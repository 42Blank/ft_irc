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
		std::string			_modeInServer_c;

	public:
		Channel();
		Channel(User &user, std::string name);
		~Channel();

		void				joinNewUser(User user);
		bool				isOperator(std::string nickname);
		bool				isUser(std::string nickname);

		std::string			getTopic();
		std::string			getUserList();
		// std::vector<User>	getRealAllUserList();
		std::vector<User>	getOperatorVector();
		std::vector<User>	getNormalUserVector();

		std::string			getChannelName();
		std::string			getModeInServer();

		void				setTopic(std::string topic);
		void				setModeInServer(std::string mode);

		void				deleteNormalUser(std::string nickname);
		int					deleteOperatorUser(std::string nickname);

};


#endif
