NAME = ircserv
#src/main.cpp\
	src/classes/Client.cpp\
	src/classes/Server.cpp\
	src/classes/ServerClients.cpp\
	src/classes/Parser.cpp\
	src/utils/sendMessage.cpp\
	src/utils/sendNumeric.cpp\
	src/classes/Channel.cpp\
	src/commands/executeJoin.cpp
# Opción 1: Usar wildcards (automático)
SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)

# SRC_MAIN = src/main.cpp
# SRC_CLASSES = $(wildcard src/classes/*.cpp)
# SRC_UTILS = $(wildcard src/utils/*.cpp)
# SRC_COMMANDS = $(wildcard src/commands/*.cpp)
# SRC = $(SRC_MAIN) $(SRC_CLASSES) $(SRC_UTILS) $(SRC_COMMANDS)

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
