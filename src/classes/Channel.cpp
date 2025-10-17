#include "Channel.hpp"

//////////////////////////////////////////////////////////////////
//																//
//							Constructors						//
//																//
//////////////////////////////////////////////////////////////////


Channel::Channel()
{
}

Channel::Channel(const std::string& name)
{
	_name = name;
	_topic = "";
	_channelKey = "";
	_limit = 0;
	_inviteOnly = false;
	_topicRestriction = false;
	
}

Channel::~Channel()
{	
}

//////////////////////////////////////////////////////////////////
//																//
//							Getters								//
//																//
//////////////////////////////////////////////////////////////////

std::string	Channel::getName() const
{
	return _name;
}

bool		Channel::isInviteOnly() const
{
	return _inviteOnly;
}

bool		Channel::isKeyProtected() const
{
	return !_channelKey.empty();
}

bool		Channel::isLimited() const
{
	return (_limit > 0);
}

bool		Channel::isTopicRestricted() const
{
	return _topicRestriction;
}

bool		Channel::isOperator(const Client& client) const
{
	std::map<std::string, Client *>::const_iterator	it = _operators.find(client.getField("NICK"));
	if (it == _operators.end())
		return false;
	return true;
}

bool		Channel::isOperator(const std::string& nick) const
{
	std::map<std::string, Client *>::const_iterator	it = _operators.find(nick);
	if (it == _operators.end())
		return false;
	return true;
}

bool		Channel::isClient(const Client& client) const
{
	std::map<std::string, Client *>::const_iterator	it = _clientChannelList.find(client.getField("NICK"));
	if (it == _clientChannelList.end())
		return false;
	return true;
}

bool		Channel::isClient(const std::string& nick) const
{
	std::map<std::string, Client *>::const_iterator	it = _clientChannelList.find(nick);
	if (it == _clientChannelList.end())
		return false;
	return true;
}

bool		Channel::isFull() const
{
	return _clientChannelList.size() >= _limit;
}

bool		Channel::isEmpty() const
{
	return _clientChannelList.empty();
}

bool		Channel::hasPass() const
{
	return !_channelKey.empty();
}

std::string	Channel::getClients() const
{
	std::string userList = "";
	std::map<std::string, Client *>::const_iterator it = _clientChannelList.begin();
	for (; it != _clientChannelList.end(); ++it)
	{
		if (!userList.empty()) userList += " ";
		// Si es operador, añadir @
		if (isOperator(it->first))
			userList += "@" + it->first;
		else
			userList += it->first;
	}

	return userList;
}

std::string	Channel::getModes() const
{
	std::string modes = "";
	if (isInviteOnly())
		modes += "i";
	if (isKeyProtected())
		modes += "k";
	if (isLimited())
		modes += "l";
	if (isTopicRestricted())
		modes += "t";
	
	return modes;
}

std::string Channel::getParameters() const
{
	std::string parameters = "";
	std::stringstream ss;

	if (isKeyProtected())
		parameters += _channelKey;
	if (isLimited())
	{
		if (!parameters.empty()) parameters += " ";
		ss << _limit;
		parameters += ss.str();
	}
	
	return parameters;
}

//////////////////////////////////////////////////////////////////
//																//
//							Member Functions					//
//																//
//////////////////////////////////////////////////////////////////

bool	Channel::addClient(const Client& client, bool makeOperator)
{
	std::string channelName = getName();
	std::string nick = client.getField("NICK");	
		
	_clientChannelList[nick] = const_cast<Client *>(&client);
	if (makeOperator)
		_operators[nick] = const_cast<Client *>(&client);
	
	_whiteList.erase(nick);

	// 1. Confirmar JOIN al cliente
	std::string joinMsg = nick + "!user@host JOIN " + channelName;
	if (!this->sendMessage(NULL, joinMsg, ":"))
		return false;
		
	// 2. Enviar lista de usuarios (RPL_NAMREPLY)
	std::string namesMsg = "353 " + nick + " = " + channelName + " :" + getClients();
	::sendMessage(":server " , client.getFd(), namesMsg);
		
	// 3. Fin de la lista (RPL_ENDOFNAMES)
	std::string endMsg = "366 " + nick + " " + channelName + " :End of /NAMES list";
	::sendMessage(":server ", client.getFd(), endMsg);

	return true;
}

bool	Channel::inviteClient(Client* client)
{
	std::string nick = client->getField("NICK");
	if (_whiteList.find(nick) != _whiteList.end())
		return true;
	_whiteList[nick] = client;
	return true;
}

bool	Channel::removeClient(const std::string& nick)
{
	removeOperator(nick);
	_clientChannelList.erase(nick);
	return true;
}

bool	Channel::changeKey(const std::string& key)
{
	this->_channelKey = key;
	return true;
}

bool	Channel::changeLimit(const std::string& limit)
{
	(void) limit;
	return true;
}

bool	Channel::makeOperator(const std::string& nick)
{
	std::map<std::string, Client *>::iterator	it = _clientChannelList.find(nick);
	if (it == _clientChannelList.end())
	{
		//error, cant make an operator as it is not a member
		return false;
	}
	this->_operators[nick] = it->second;
	return true;
}

bool	Channel::removeOperator(const std::string& nick)
{
	_operators.erase(nick);
	return true;
}

bool	Channel::inviteOnly(bool inviteOnly)
{
	_inviteOnly = inviteOnly;
	return true;
}

bool	Channel::topicRestriction(bool topicRestriction)
{
	_topicRestriction = topicRestriction;
	return true;
}

bool	Channel::sendMessage(Client *client, const std::string& msg, const std::string& prefix) const
{
	if (client && !isClient(*client))
	{
		if (!sendNumeric(client, 404, getName()))
			return (false);
		return (true);
	}
	std::map<std::string, Client *>::const_iterator	it = _clientChannelList.begin();
	std::map<std::string, Client *>::const_iterator	end = _clientChannelList.end();
	for (; it != end; ++it)
	{
		if (client && client->getField("NICK") == it->second->getField("NICK"))
			continue;
		if (!(::sendMessage(prefix, it->second->getFd(), msg)))
			return (false);
	}
	return (true);
}

