#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

static bool	isValid(const ParsedCommand& cmd)
{
	int size = cmd.params.size();
	if (size != 4)
		return false;
	return true;
}

bool	Server::executeKick(Client* client, const ParsedCommand& cmd)
{
	if (!isValid(cmd))
		return false;
	std::map<std::string, Channel *>::iterator	it = this->_channelMap.find(cmd.params[1]);
	if (it == this->_channelMap.end())
		return false;
	if (it->second->isOperator(*client) && it->second->isClient(cmd.params[2]))
		return it->second->removeClient(cmd.params[2], cmd.params[3]);
	return false;
}