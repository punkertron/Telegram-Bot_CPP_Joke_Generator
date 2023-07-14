NAME		= telegram_bot
SRCS		= request.cpp main.cpp
OBJS		= ${SRCS:.cpp=.o}
INCLUDES	= -I/usr/local/include

LDLIBS		= -lTgBot -lboost_system -lssl -lcrypto -lpthread -lcurl

DEPS		= ${OBJS:%.o=%.d}

CXX			= g++
CXXFLAGS	= -std=c++14 -fsanitize=address -fsanitize=leak -fsanitize=undefined

RM			= rm -rf

all: ${NAME}

${NAME}: ${OBJS}
		${CXX} ${CXXFLAGS} ${INCLUDES} ${OBJS} ${LDLIBS} -o ${NAME}

%.o:%.cpp
		${CXX} ${CXXFLAGS} ${INCLUDES} -MMD -c $< -o $@

-include ${DEPS}

clean:
		${RM} ${OBJS} ${DEPS}

fclean: clean
		${RM} ${NAME}

re: fclean ${NAME}

.PHONY: all clean fclean re