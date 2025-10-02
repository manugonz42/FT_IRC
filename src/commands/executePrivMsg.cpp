#include "Ircserv.hpp"

int	commandIsValid(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() != 3)
	{
		if (cmd.params[1][0] == ':')
		{
			if (!sendNumeric(client, 411, ""))
				return (-1);
		}
		else
		{
			if (sendNumeric(client, 412, ""))
				return (-1);
		}	
		return (0);
	}
	if (cmd.params[2][0] != ':' || cmd.params[2].size() == 1)
	{
		if (sendNumeric(client, 412, ""))
			return (-1);
		return (0);
	}
	return (1);
}

bool	Server::executePrivMsg(Client *client, const ParsedCommand &cmd)
{
	int	validCmd = commandIsValid(client, cmd);
	if (validCmd)
	{
		std::string prefix = ":" + client->getField("NICK") + "!" + client->getField("USER") + ":" + client->getField("HOST");
		if (cmd.params[1][0] == '#')
		{
			std::map<std::string, Channel *>::iterator i = _channelMap.find(cmd.params[1]);
			if (i == _channelMap.end())
			{
				if (sendNumeric(client, 401/*403*/, cmd.params[1]))
					return (false);
				return (true);
			}
			Channel *channel = i->second;
			channel->sendMessage(prefix ,cmd.params[2]);
		}
		else
		{
			std::map<std::string, Client *>::iterator i;
			i = _clientMap.find(cmd.params[1]);
			if (i == _clientMap.end())
			{
				if (sendNumeric(client, 401, cmd.params[1]))
					return(false);
				return (true);
			}
			::sendMessage(prefix, i->second->getFd(), cmd.params[2]);
		}
	}
	else if (validCmd == -1)
		return (false);
	return (true);
}
