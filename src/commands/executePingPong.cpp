
#include "Ircserv.hpp"

bool	Server::executePingPong(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() < 2)
	{
		if (::sendNumeric(client, 409, ""));
		return (false);
	}
	::sendMessage(PREFIX, client->getFd(), "PONG " + cmd.params[1]);
	return (true);
}