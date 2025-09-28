#include "Ircserv.hpp"

bool	pingPong(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() < 2)
		sendMessage(client->getFd(), "409 " + client->getField("NICK") + " :No origin specified");
	sendMessage(client->getFd(), "PONG " + cmd.params[1]);
}