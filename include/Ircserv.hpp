#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <vector>
# include <iostream>
# include <csignal>
# include <sys/signal.h>
# include <sys/socket.h>
# include <sys/poll.h>
# include <errno.h>
# include <netinet/in.h>
# include <unistd.h>
# include <cstring>
# include <stdio.h>
# include <cstdlib>
# include <fcntl.h>
# include "Structures.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Parser.hpp"
# include "Server.hpp"
# include "commands.hpp"

#define VALID_IRC_COMMANDS \
    "CAP", "PASS", "NICK", "USER", "JOIN", "PART", \
    "PRIVMSG", "NOTICE", "QUIT", "PING", "PONG", "VERSION", "TIME", "MODE"

#define VALID_IRC_COMMANDS_COUNT 14

typedef enum e_command
{
	CAP,
	PASS,
    NICK,
    USER,
    JOIN,
    PART,
    PRIVMSG,
    NOTICE,
    QUIT,
    PING,
    PONG,
    VERSION,
    TIME,
}	t_command;

int	sendMessage(int fd ,const std::string &msg);

#endif
