##
## Makefile for Babel Server in /home/thai_j//Babel/babel-2014-phol_r/Server/Babel server
## 
## Made by jean thai
## Login   <thai_j@epitech.net>
## 
## Started on  Fri Dec  2 18:28:26 2011 jean thai
## Last update Fri Dec  2 18:50:59 2011 jean thai
##

NAME		= BabelServer
SRCS		= ClientManager.cpp	\
		  main.cpp		\
		  NetworkManager.cpp	\
		  QueryManager.cpp	\
		  SClient.cpp		\
		  Server.cpp

OBJS		= $(SRCS:.cpp=.o)
CXX		= g++
RM		= rm -f

$(NAME)		: $(OBJS)
		$(CXX) -lboost_system-mt -lboost_thread-mt -o $(NAME) $(OBJS)

all		: $(NAME)

clean		:
		$(RM) $(OBJS)

fclean		: clean
		$(RM) $(NAME)

re		: fclean all

.PHONY		: all clean fclean re