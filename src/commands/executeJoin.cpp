#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"



/*
	Falta implementar unirse con contraseña y solo invitacion
*/
		
bool	Server::executeJoin(Client *client, const ParsedCommand &cmd)
{
	std::vector<std::string> channels = parseChannels(cmd.params[1]);
	
	if (channels.empty())
	{
		if (!sendNumeric(client, 403, cmd.params[1]))
			return false;
		return true;
	}
	
	int	numChannels = channels.size();
	std::cout << numChannels << std::endl;
	for (int i = 0; i < numChannels; i++)
	{
		
		std::string channelName = channels[i];
		std::cout << channelName << std::endl;
		std::map<std::string, Channel *>::iterator	it = _channelMap.find(channelName);
		if (it == _channelMap.end())
		{
			if (!createChannel(*client, channelName))
				return false;
		}
		else if (!it->second->isInviteOnly() && !it->second->isFull())
		{
			if(!it->second->addClient(*client, false))
				return false;
		}
	}
	return true;
}

bool	Server::createChannel(const Client& client, const std::string& name)
{
	Channel*	ch = new Channel(name);
	if (!ch->addClient(client, true))
		return false;
	_channelMap.insert(std::make_pair(name, ch));

	return true;
}
