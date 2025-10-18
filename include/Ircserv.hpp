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
# include <set>
# include "Colors.hpp"
# include "Defines.hpp"
# include "Structures.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Parser.hpp"
# include "Server.hpp"

std::string strToUpper(const std::string& str);
bool isValidLength(const std::string &str, size_t minLen, size_t maxLen);

int	sendMessage(std::string prefix, int fd ,const std::string &msg);
int	sendNumeric(Client *client, int numeric, const std::string &target);
std::vector<std::string> parseChannels(const std::string& channelParam);

void	signalHandler(int signal);
#endif
