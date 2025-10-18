#include "Ircserv.hpp"

bool	Server::executePingPong(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() < 2)
	{
		if (!sendNumeric(client, 409, ""))
			return (false);
		return (true);
	}
	if (!::sendMessage(PREFIX, client->getFd(), "PONG " + cmd.params[1]))
		return (false);
	return (true);
}
