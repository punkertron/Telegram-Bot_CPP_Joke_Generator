NAME		= telegram_bot

SRCS_PATH	= ./srcs
SRCS_FILES	= main.cpp request.cpp response.cpp bot.cpp utils.cpp
SRCS		= ${addprefix ${SRCS_PATH}/, ${SRCS_FILES}}

OBJS_PATH	= ./objs
OBJS_FILES	= ${SRCS_FILES:.cpp=.o}
OBJS		= ${addprefix ${OBJS_PATH}/, ${OBJS_FILES}}

INC_DIR		= ./incs
INC			= /usr/local/include ./nlohmann-json ${INC_DIR}
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

format:
	clang-format -i ${SRCS} ${INC_DIR}/*

.PHONY: all clean fclean re format
