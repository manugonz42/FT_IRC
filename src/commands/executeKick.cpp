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
	{
		if (!sendNumeric(client, 461, "KICK"))
			return false;
		return true;
	}
	std::map<std::string, Channel *>::iterator	it = this->_channelMap.find(cmd.params[1]);
	if (it == this->_channelMap.end())
	{
		if (!sendNumeric(client, 403, cmd.params[1]))
			return false;
		return true;
	}

	Channel* ch = it->second;

	if (!ch->isOperator(*client))
	{
		if (!sendNumeric(client, 482, cmd.params[1]))
			return false;
		return true;
	}

	if (!ch->isClient(cmd.params[2]))
	{
		if (!sendNumeric(client, 441, cmd.params[2] + " " + cmd.params[1]))
			return false;
		return true;
	}
	std::string	kickPrefix = ":" + client->getField("NICK") + "!user@host ";
	std::string kickMsg = "KICK " + cmd.params[1] + " " + cmd.params[2] + " :" + cmd.params[3];
	if(!ch->sendMessage(NULL, kickMsg, kickPrefix))
		return false;
	ch->removeClient(cmd.params[2]);
	return true;
}