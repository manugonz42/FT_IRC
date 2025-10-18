NAME = ircserv
SRC = \
	src/main.cpp \
	src/executeCommand.cpp \
	src/classes/Channel.cpp \
	src/classes/Client.cpp \
	src/classes/Parser.cpp \
	src/classes/ServerClients.cpp \
	src/classes/Server.cpp \
	src/commands/executeCap.cpp \
	src/commands/executeInvite.cpp \
	src/commands/executeJoin.cpp \
	src/commands/executeKick.cpp \
	src/commands/executeMode.cpp \
	src/commands/executeNick.cpp \
	src/commands/executeNotice.cpp \
	src/commands/executePart.cpp \
	src/commands/executePass.cpp \
	src/commands/executePingPong.cpp \
	src/commands/executePrivMsg.cpp \
	src/commands/executeQuit.cpp \
	src/commands/executeTopic.cpp \
	src/commands/executeUser.cpp \
	src/commands/executeVersion.cpp \
	src/utils/isValidLength.cpp \
	src/utils/parseChannels.cpp \
	src/utils/sendMessage.cpp \
	src/utils/sendNumeric.cpp \
	src/utils/sendWellcome.cpp \
	src/utils/strToUpper.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -Iinclude
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
