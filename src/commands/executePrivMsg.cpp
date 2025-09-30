#include "Ircserv.hpp"
bool	Server::executePrivMsg(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() != 3)
	{
		if (cmd.params[1][0] == ':')
			if (!sendNumeric(client, 411, ""))
				return (false);
		else
			if (sendNumeric(client, 412, ""))
				return (false);
		return (true);
	}
	if (cmd.params[1][0] == '#')
	{
		std::map<std::string, Channel *>::iterator i = _channelMap.find(cmd.params[1]);
		if (i == _channelMap.end())
		{
			if (sendNumeric(client, 403, cmd.params[1]))
				return (false);
		}
		Channel *channel = i->second;
		channel->sendMessage(cmd.params[2]);
	}
}