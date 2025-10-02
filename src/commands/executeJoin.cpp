#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"



/*
	Falta implementar varias funcionalidades de join, pero 
	debería poder unirse a un canal.
*/
//
// DESDE AQUI ------------------------------
//
static bool	isValid(const std::string& channel)
{
    if (channel.empty() || channel[0] != '#')
        return false;
    return true;
}

std::vector<std::string> parseChannels(const std::string& channelParam) {
    std::vector<std::string> channels;
    
    if (channelParam.empty()) {
        return channels;
    }
    
    std::string current = "";
    
    for (size_t i = 0; i < channelParam.length(); ++i) {
        char c = channelParam[i];
        
        if (c == ',') {
            if (!current.empty()) {
                channels.push_back(current);
                current = "";
            }
        } else 
            current += c;
        
    }
    
    if (!current.empty()) {
        channels.push_back(current);
    }
    
    return channels;
}

bool	Server::executeJoin(Client *client, const ParsedCommand &cmd)
{
//	cmd[1] = nombre del canal;
	if (cmd.params.size() < 2)
        return true;

	std::vector<std::string> channels = parseChannels(cmd.params[1]);
	
	// Validar cada canal del vector
	for (size_t i = 0; i < channels.size(); ++i)
	{
		if (!isValid(channels[i]))
		{
			return true;
		}
	}
	
	if (channels.empty()) {
		return true;
	}
	
	// Procesar solo el primer canal (filtro temporal)
	std::string channelName = channels[0];
//
// HASTA AQUI ------------------------------
//	
	std::map<std::string, Channel *>::iterator	it = _channelMap.find(channelName);
	if (it == _channelMap.end())
		return createChannel(*client, channelName);
	if (!it->second->isInviteOnly() && !it->second->isFull())
			return it->second->addClient(*client, false);
	return true;
}

bool	Server::createChannel(const Client& client, const std::string& name)
{
	Channel*	ch = new Channel(name);
	ch->addClient(client, true);
	_channelMap.insert(std::make_pair(name, ch));

	return true;
}
