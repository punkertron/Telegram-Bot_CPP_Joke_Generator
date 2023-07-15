NAME		= telegram_bot

SRCS_PATH	= ./srcs
SRCS_FILES	= main.cpp request_response.cpp bot.cpp utils.cpp

OBJS_PATH	= ./objs
OBJS_FILES	= ${SRCS_FILES:.cpp=.o}
OBJS		= ${addprefix ${OBJS_PATH}/, ${OBJS_FILES}}

INC			= /usr/local/include ./incs
INCLUDES	= $(INC:%=-I %)

LDLIBS		= -lTgBot -lboost_system -lssl -lcrypto -lpthread -lcurl

DEPS		= ${OBJS:%.o=%.d}

CXX			= g++
CXXFLAGS	= -std=c++14 -O2 #-fsanitize=address -fsanitize=leak -fsanitize=undefined

RM			= rm -rf

all: ${NAME}

${NAME}: ${OBJS}
		${CXX} ${CXXFLAGS} ${INCLUDES} ${OBJS} ${LDLIBS} -o ${NAME}

${OBJS_PATH}/%.o : ${SRCS_PATH}/%.cpp | ${OBJS_PATH}
		${CXX} ${CXXFLAGS} ${INCLUDES} -MMD -c $< -o $@

${OBJS_PATH}:
	mkdir -p ${OBJS_PATH}

-include ${DEPS}

clean:
		${RM} ${OBJS_PATH}

fclean: clean
		${RM} ${NAME}

re: fclean ${NAME}

.PHONY: all clean fclean re