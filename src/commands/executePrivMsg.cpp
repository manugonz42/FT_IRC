#include "Ircserv.hpp"

static int	commandIsValid(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() <= 1)
	{
		if (!sendNumeric(client, 461, "PRIVMSG :Not enough parameters"))
			return (-1);
		return (0);
	}
	if (cmd.params.size() != 3)
	{
		if (cmd.params[1][0] == ':')
		{
			if (!sendNumeric(client, 411, ""))
				return (-1);
		}
		else
		{
			if (!sendNumeric(client, 412, ""))
				return (-1);
		}
		return (0);
	}
	if (cmd.params[2][0] != ':' || cmd.params[2].size() == 1)
	{
		if (!sendNumeric(client, 412, ""))
			return (-1);
		return (0);
	}
	return (1);
}

bool	Server::executePrivMsg(Client *client, const ParsedCommand &cmd)
{
	int			validCmd = commandIsValid(client, cmd);
	if (validCmd == -1)
		return (false);
	if (!validCmd)
		return (true);
	std::string	text = cmd.params[1] + " " + cmd.params[2];
	if (cmd.params[1][0] == '#')
	{
		std::map<std::string, Channel *>::iterator i = _channelMap.find(cmd.params[1]);
		if (i == _channelMap.end())
		{
			if (!sendNumeric(client, 403, cmd.params[1]))
				return (false);
			return (true);
		}
		Channel *channel = i->second;
		if (!channel->sendMessage(client, "PRIVMSG " + text, client->getField("PREFIX")))
			return (false);
	}
	else
	{
		std::map<std::string, Client *>::iterator i;
		i = _clientMap.find(strToUpper(cmd.params[1]));
		if (i == _clientMap.end())
		{
			if (!sendNumeric(client, 401, cmd.params[1]))
				return(false);
			return (true);
		}
		if (!::sendMessage(client->getField("PREFIX"), i->second->getFd(), "PRIVMSG " + text))
			return (false);
	}
	return (true);
}
