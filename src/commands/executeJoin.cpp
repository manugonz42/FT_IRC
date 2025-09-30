#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"



static bool	isValid(const ParsedCommand& cmd)
{
	return true;
}


bool	executeJoin(Client *client, const ParsedCommand &cmd)
{
//	cmd[1] = nombre del canal;
	if (!isValid(cmd))
		return false;
	for (Channel ch : this->_channelList)
	{
		if (cmd[1] == ch->getName())
		{
			if (!ch->isInviteOnly())
				ch->addClient(*client);
			return true;
		}
	}
	createChannel(client, cmd[1]);
	return true;
}
