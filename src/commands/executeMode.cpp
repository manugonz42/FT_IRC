#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

bool	Server::executeMode(Client *client, const ParsedCommand &cmd)
{
	(void)client;
	(void)cmd;
	return true;
}