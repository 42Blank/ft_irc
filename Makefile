# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jiychoi <jiychoi@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/28 13:47:07 by jiychoi           #+#    #+#              #
#    Updated: 2023/01/08 00:42:57 by jiychoi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
SRCS		=	src/main.cpp \
				src/Server.cpp src/ServerSendMessage.cpp src/ServerUtils.cpp \
				src/User.cpp src/Channel.cpp \
				src/Command.cpp src/CommandChannel.cpp src/CommandAuth.cpp \
				src/utils.cpp src/Reply.cpp src/Bot.cpp

OBJS		=	$(SRCS:.cpp=.o)
TARGET		=	ircserv
SANFLAG		=	-g3 -fsanitize=address

NO_COLOR	=	\e[0m
LF			=	\e[1K\r$(NO_COLOR)
CRLF		=	\n$(LF)

all : $(TARGET)

$(TARGET) : $(OBJS)
	@printf "$(LF)📞ft_irc\n"
	@$(CXX) $(OBJS) $(SANFLAG) $(CXXFLAGS) -o $(TARGET)

%.o : %.cpp
	@$(CXX) $(CXXFLAGS) -c -o $@ $< -I ./
# @$(CXX) $(CXXFLAGS) $(SANFLAG) -c -o $@ $< -I ./

clean :
	@printf "$(LF)Deleting $(TARGET)...\n"
	@rm -rf $(OBJS)

fclean : clean
	@rm -rf $(TARGET)

re : fclean all
