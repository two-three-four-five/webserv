SRCS = $(wildcard *.cpp) \
		$(wildcard */*.cpp)
OBJS = $(SRCS:.cpp=.o)
NAME = webserv
# CXXFLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDE = src

.PHONY : clean all fclean re

all : $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $(NAME) $(CXXFLAGS) $(OBJS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I $(INCLUDE)

clean :
	rm -f $(OBJS)

fclean :
	rm -f $(OBJS)
	rm -f $(NAME)

re :
	make fclean
	make all
