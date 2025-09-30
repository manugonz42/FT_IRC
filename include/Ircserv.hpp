#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <vector>
# include <iostream>
# include <csignal>
# include <cctype>
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
# include <map>
# include "Structures.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Parser.hpp"
# include "Server.hpp"
# include "commands.hpp"

#define PREFIX ":server "

#define VALID_IRC_COMMANDS \
    "CAP", "PASS", "NICK", "USER", "JOIN", "PART", "KICK", "INVITE", "TOPIC", \
    "PRIVMSG", "NOTICE", "QUIT", "PING", "VERSION", "TIME", "MODE"

#define VALID_IRC_COMMANDS_COUNT 16

enum LoginStatus
{
    UNREGISTERED,   // Recién conectado
    PASS_SENT,      // Contraseña enviada
    NICK_SENT,      // Nickname enviado  
    USER_SENT,      // Usuario enviado (NUEVO)
    REGISTERED      // Login completo
};
#define VALID_IRC_COMMANDS_COUNT 16

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
int	sendNumeric(Client *client, int numeric, const std::string &target);
#endif
