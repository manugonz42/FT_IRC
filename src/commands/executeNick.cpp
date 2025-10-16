#include "Ircserv.hpp"


bool isSpecial(char c) 
{
	return c == '[' || c == ']' || c == '\\' || c == '`' ||
		c == '-' || c == '^' || c == '{' || c == '}' || c == '_';
}

bool isValidNickname(const std::string &nick)
{
	if (nick.empty() || nick.length() > 9)
		return false;

	if (!std::isalpha(nick[0]) && !isSpecial(nick[0]))
		return false;

	for (size_t i = 1; i < nick.length(); i++)
	{
		char c = nick[i];
		if (!std::isalnum(c) && !isSpecial(c))
			return false;
	}

	return true;
}

int isValidNickCommand(Client *client, const ParsedCommand &cmd)
{
	if (cmd.params.empty() || cmd.params.size() < 2)
	{
		if (!sendNumeric(client, 431, ":No nickname given"))
			return (-1);
		return false;
	}
	if (cmd.params.size() > 2)
	{
		if (!sendNumeric(client, 461, "NICK :Too many parameters"))
			return (-1);
		return false;
	}
	return true;
}


bool Server::executeNick(Client *client, const ParsedCommand &cmd)
{
	int	valid;
	valid = isValidNickCommand(client, cmd);
	if (valid == -1)
		return false;
	if (!valid)
		return true;
	
	if (!isValidNickname(cmd.params[1]))
		return (sendNumeric(client, 432, cmd.params[1]));

	std::string upperNick = strToUpper(cmd.params[1]);

	std::map<std::string, Client *>::iterator it = _clientMap.find(upperNick);
	if (it != _clientMap.end())
	{
		if (client->getLoginStatus() >= REGISTERED)
		{
			if (it->second == client)
			{
				client->setField("NICK", cmd.params[1]);
				return true; // Mismo cliente, mismo nick, ignorar
			}
			else
				return (sendNumeric(client, 433, cmd.params[1]));
		}
		else
		{
			return (sendNumeric(client, 433, cmd.params[1]));
		}
		
		return true;
	}
	
	if (client->getLoginStatus() < REGISTERED)
	{
		client->setField("NICK", cmd.params[1]);
		_clientMap[upperNick] = client;
		if (client->getLoginStatus() == USER_SENT)
		{
			client->setLoginStatus(REGISTERED);
			if (!sendWelcome(client))
				return false;
			return true;
		}
		client->setLoginStatus(NICK_SENT);
		
		return true;
	}
	
	else
	{
		std::string	prefix = client->getField("PREFIX");
		std::string oldNick = client->getField("NICK");
		client->setField("NICK", cmd.params[1]);
		std::string oldUpperNick = strToUpper(oldNick);
		_clientMap.erase(oldUpperNick);     
		_clientMap[upperNick] = client;     
		
		if (!::sendMessage(prefix, client->getFd(), "NICK " + cmd.params[1]))
			return false;
		for (std::map<std::string, Channel *>::iterator it = _channelMap.begin();
			it != _channelMap.end(); ++it)
		{
			Channel *channel = it->second;
			if (channel->isClient(oldNick))
			{
				channel->renameClient(oldNick, client->getField("NICK"));
			}
		}
		if (!notifyToAllChannels(prefix, client, "NICK " + cmd.params[1]))
			return (false);
		client->setPrefix();
		return true;
	}
}