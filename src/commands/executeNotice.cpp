#include "Ircserv.hpp"

static int	commandIsValid(const ParsedCommand &cmd)
{
	if (cmd.params.size() != 3)
		return (0);
	if (cmd.params[2][0] != ':' || cmd.params[2].size() == 1)
		return (0);
	return (1);
}

bool	Server::executeNotice(Client *client, const ParsedCommand &cmd)
{
	int	validCmd = commandIsValid(cmd);
	if (!validCmd)
		return (true);
	std::string	text = cmd.params[1] + " " + cmd.params[2];
	std::string prefix = client->getField("PREFIX");
	if (cmd.params[1][0] == '#')
	{
		std::map<std::string, Channel *>::iterator i = _channelMap.find(cmd.params[1]);
		if (i == _channelMap.end())
			return (true);
		Channel *channel = i->second;
		if (!channel->isClient(*client))
			return true;

		channel->sendMessage(client, "NOTICE " + text, prefix);
	}
	else
	{
		std::map<std::string, Client *>::iterator i;
		i = _clientMap.find(strToUpper(cmd.params[1]));
		if (i == _clientMap.end())
			return (true);
		if (!::sendMessage(prefix, i->second->getFd(), "NOTICE " + text))
			return (false);
	}
	return (true);
}
