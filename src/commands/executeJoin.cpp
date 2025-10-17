#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"



/*
	Falta implementar unirse con contraseña y solo invitacion
*/

std::vector<std::string> parseKeys(const std::string& param) {
	std::vector<std::string> keys;
		
	if (param.empty())
		return keys;
		
	std::string current = "";
	for (size_t i = 0; i < param.length(); ++i) {
		char c = param[i];
		
		if (c == ',') {
			if (!current.empty()) {
				keys.push_back(current);
				current = "";
			}
		} else 
			current += c;
		
	}

	if (!current.empty())
		keys.push_back(current);
		
	return keys;
}

bool	Server::executeJoin(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.size() < 2)
	{
		if (!sendNumeric(client, 461, "JOIN :Not enough parameters"))
			return false;
		return true;
	}

	std::vector<std::string>	channels = parseChannels(cmd.params[1]);
	std::vector<std::string>	keys;
	if (cmd.params.size() > 2)
		keys = parseKeys(cmd.params[2]);
	
	if (channels.empty())
	{
		if (!sendNumeric(client, 403, cmd.params[1]))
			return false;
		return true;
	}
	
	size_t	numChannels = channels.size();
	for (size_t i = 0; i < numChannels; i++)
	{
		
		std::string channelName = channels[i];
		std::string	key = (i < keys.size()) ? keys[i] : "";

		std::map<std::string, Channel *>::iterator	it = _channelMap.find(channelName);
		if (it == _channelMap.end())
		{
			if (!createChannel(*client, channelName))
				return false;
		}
		else
		{
			Channel*	channel = it->second;

			//Verificar si ya se ha unido
			if (channel->isClient(*client))
			{
				if (!sendNumeric(client, 443, channelName))
					return false;
				continue;
			}

			//Verificar si esta baneado
			if (channel->isBanned(*client))
			{
				if (!sendNumeric(client, 474, channelName))
					return false;
				continue;
			}

			//Verificar si esta lleno
			if (channel->isFull())
			{
				if (!sendNumeric(client, 471, channelName))
					return false;
				continue;
			}

			//Verificar contraseña
			if (channel->hasKey() && !channel->introduceKey(key))
			{
				if (!sendNumeric(client, 475, channelName))
					return false;
				continue;
			}

			//Verificar solo invitacion
			if (channel->isInviteOnly() && !channel->isInvited(*client))
			{
				if (!sendNumeric(client, 473, channelName))
					return false;
				continue;
			}
			
			if (!channel->addClient(*client, false))
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
