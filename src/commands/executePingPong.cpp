
#include "Ircserv.hpp"

bool	Server::executePingPong(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() < 2)
	{
		sendMessage(PREFIX, client->getFd(), "409 " + client->getField("NICK") + " :No origin specified");
		return (false);
	}
	sendMessage(PREFIX, client->getFd(), "PONG " + cmd.params[1]);
	return (true);
}