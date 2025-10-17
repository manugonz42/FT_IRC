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
		//error parametros no validos 461
		return true;
	}

	std::map<std::string, Channel*>::iterator itChannel = _channelMap.find(cmd.params[2]);
	if (itChannel == _channelMap.end())
	{
		//error no existe el canal 403
		return true;
	}

	Channel*	ch = itChannel->second;
	
	if (!ch->isOperator(*client))
	{
		//error no eres operador 482
		return true;
	}

	if (ch->isClient(cmd.params[1]))
	{
		//error ya es un cliente del canal 443
		return true;
	}

	std::map<std::string, Client*>::iterator itCliente = _clientMap.find(cmd.params[1]);
	if (itCliente == _clientMap.end())
	{
		//error usuario no existe 401
		return true;
	}

	ch->inviteClient(itCliente->second);

	//mensaje de invitacion al cliente destino

	return true;
}