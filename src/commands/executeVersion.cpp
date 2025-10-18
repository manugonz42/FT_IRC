#include "Ircserv.hpp"

bool Server::executeVersion(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() == 1)
	{
		if (!sendNumeric(client, 351, std::string(SERVER_VERSION) + "." + DEBUGLEVEL + " " + _serverName + " :" + BLUE + COMMENTS + RESET))
			return false;
		return true;
	}
	
	if (cmd.params.size() == 2)
	{
		std::string target = cmd.params[1];
		
		if (target == _serverName || target == "localhost" || target == "127.0.0.1")
		{

			if (!sendNumeric(client, 351, std::string(SERVER_VERSION) + "." + DEBUGLEVEL + " " + _serverName + " :" + BLUE + COMMENTS + RESET))
				return false;
			return true;
		}
		
		if (!sendNumeric(client, 402, " :No such server"))
			return false;
		return true;
	}
	
	if (!sendNumeric(client, 461, "VERSION :Too much parameters"))
		return false;
	return true;
}