#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

static bool	isValid(const ParsedCommand& cmd)
{
	return (cmd.params.size() == 3 && cmd.params[2][0] == '#');
}


bool	Server::executeInvite(Client *client, const ParsedCommand &cmd)
{
	if (!isValid(cmd))
	{
		if (!sendNumeric(client, 461, "KICK :Invalid parameters"))
			return false;
		return true;
	}

	std::map<std::string, Channel*>::iterator itChannel = _channelMap.find(cmd.params[2]);
	if (itChannel == _channelMap.end())
	{
		if (!sendNumeric(client, 403, cmd.params[2]))
			return false;
		return true;
	}

	Channel*	ch = itChannel->second;
	
	if (!ch->isOperator(*client))
	{
		if (!sendNumeric(client, 482, cmd.params[2]))
			return false;
		return true;
	}

	if (ch->isClient(cmd.params[1]))
	{
		if (!sendNumeric(client, 443, cmd.params[1] + " " + cmd.params[2]))
			return false;
		return true;
	}

	std::map<std::string, Client*>::iterator itCliente = this->_clientMap.find(strToUpper(cmd.params[1]));
	if (itCliente == _clientMap.end())
	{
		if (!sendNumeric(client, 401, cmd.params[1]))
			return false;
		return true;
	}

	ch->inviteClient(itCliente->second);

	std::string	sendMsg = "INVITE " + cmd.params[1] + " " + cmd.params[2];
	if(!::sendMessage(client->getField("PREFIX"), itCliente->second->getFd(), sendMsg))
		return false;
	if (!sendNumeric(client, 341, cmd.params[1] + " " + cmd.params[2]))
		return false;

	return true;
}