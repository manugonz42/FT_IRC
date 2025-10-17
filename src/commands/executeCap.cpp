#include "Ircserv.hpp"

bool	Server::executeCap(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() < 2)
		return (sendNumeric(client, 461, "CAP :Not enough parameters"));
	if (cmd.params[1] == "LS")
	{
		if (!::sendMessage(PREFIX, client->getFd(), "CAP * LS :"))
			return (false);
	}
	else if (cmd.params[1] == "END")
	{
		if (!::sendMessage(PREFIX, client->getFd(), "CAP * ACK :"))
			return (false);
	}
	else
		return (sendNumeric(client, 461, "CAP :Argument not handled"));
	return (true);
}