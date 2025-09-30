#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"



/*
	Falta implementar varias funcionalidades de join, pero 
	debería poder unirse a un canal.
*/


static bool	isValid(const ParsedCommand& cmd)
{
	if (cmd.params.size() < 2)
        return false;
    if (cmd.params[1].empty() || cmd.params[1][0] != '#')
        return false;
    return true;
}


bool	Server::executeJoin(Client *client, const ParsedCommand &cmd)
{
//	cmd[1] = nombre del canal;
	if (!isValid(cmd))
		return true;
	std::map<std::string, Channel *>::iterator	it = _channelMap.find(cmd.params[1]);
	if (it == _channelMap.end())
		return createChannel(*client, cmd.params[1]);
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
