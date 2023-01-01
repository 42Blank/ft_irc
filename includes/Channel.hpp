#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "User.hpp"

# include <string>
# include <vector>

class Channel {
	private:
		std::string		_operator;	// user의 nickname 저장
		std::string		_topic;		
		std::vector<User>	_userList;

	public:
		Channel(User &user);
		~Channel();

		void	Channel::joinNewUser(User user);
		bool	Channel::isOperator(User user);

		std::string			Channel::getTopic();
		std::vector<User>	Channel::getuserList();

		void				Channel::setTopic(std::string topic);
		
		void				Channel::deleteNormalUser(std::string nickname);
		void				Channel::deleteOperatorUser(std::string nickname);

};


#endif