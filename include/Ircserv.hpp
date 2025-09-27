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
# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"


typedef enum e_command
{
	PRIVMSG,
	USER,
	JOIN
}	t_command;

#endif
