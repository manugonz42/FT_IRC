#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

static bool	isValid(const ParsedCommand& cmd)
{
	return (cmd.params.size() >= 2 && cmd.params.size() <= 3);
}


bool	Server::executePart(Client *client, const ParsedCommand &cmd)
{
	if (!isValid(cmd))
	{
		if (!sendNumeric(client, 461, "PART :Invalid number of paramaters"))
			return false;
		return true;
	}
	
	std::vector<std::string> channels = parseChannels(cmd.params[1]);
	
	if (channels.empty())
	{
		if (!sendNumeric(client, 403, cmd.params[1]))
			return false;
		return true;
	}

	int numChannels = channels.size();

	for (int i = 0; i < numChannels; i++)
	{
		std::string channelName = channels[i];

		std::map<std::string, Channel *>::iterator	it = _channelMap.find(channelName);
		if (it == _channelMap.end())
		{
			if (!sendNumeric(client, 403, channelName))
				return false;
		}
		else
		{
			if (!it->second->isClient(*client))
			{
				if (!sendNumeric(client, 442, channelName))
					return false;
			}
			else
			{
				std::string	partPrefix = client->getField("PREFIX");
				std::string partMsg = "PART " + channelName;
				if (cmd.params.size() > 2)
					partMsg += " :" + cmd.params[2];
				if(!it->second->sendMessage(NULL, partMsg, partPrefix))
					return false;
				it->second->removeClient(client->getField("NICK"));
			}
		}
	}
	return true;
}