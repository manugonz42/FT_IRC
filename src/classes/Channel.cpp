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
	_topic = "Bienvenidos al canal " + name;
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

bool		Channel::isInvited(const Client& client) const
{
	return _whiteList.find(client.getField("NICK")) != _whiteList.end();
}

bool		Channel::isBanned(const Client& client) const
{
	return _blackList.find(client.getField("NICK")) != _blackList.end();
}

bool		Channel::isFull() const
{
	if (_limit == 0)
		return false;
	return _clientChannelList.size() >= _limit;
}

bool		Channel::isEmpty() const
{
	return _clientChannelList.empty();
}

bool		Channel::hasKey() const
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
	if (hasKey())
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

	if (hasKey())
		parameters += _channelKey;
	if (isLimited())
	{
		if (!parameters.empty()) parameters += " ";
		ss << _limit;
		parameters += ss.str();
	}
	
	return parameters;
}

std::string	Channel::getTopic() const
{
	return _topic;
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
	std::string joinMsg = " JOIN " + channelName;
	if (!this->sendMessage(NULL, joinMsg, client.getField("PREFIX")))
		return false;
		
	// 2. Enviar lista de usuarios (RPL_NAMREPLY)
	std::string namesMsg = "353 " + nick + " = " + channelName + " :" + getClients();
	if(!::sendMessage(PREFIX , client.getFd(), namesMsg))
		return false;
		
	// 3. Fin de la lista (RPL_ENDOFNAMES)
	std::string endMsg = "366 " + nick + " " + channelName + " :End of /NAMES list";
	if (!::sendMessage(PREFIX, client.getFd(), endMsg))
		return false;

	std::cout << "Se añade al canal: " + channelName + " al usuario: " + nick << std::endl;
	return true;
}

bool	Channel::inviteClient(Client* client)
{
	std::string nick = client->getField("NICK");
	_whiteList.insert(nick);
	return true;
}

bool	Channel::bannClient(const std::string& nick)
{
	_blackList.insert(nick);
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

bool	Channel::introduceKey(const std::string& key) const
{
	return _channelKey == key;
}

bool	Channel::changeLimit(const std::string& limit)
{
	if (limit.empty())
	{
		_limit = 0;  // Sin límite
		return true;
	}
		
	std::stringstream ss(limit);
	size_t newLimit;
		
	// Intentar convertir
	if (!(ss >> newLimit))
		return false;  // Conversión falló
		
	// Verificar que no queden caracteres sin procesar
	std::string remainder;
	if (ss >> remainder)
		return false;  // Había caracteres no numéricos
		
	// Validar rango razonable
	if (newLimit == 0 || newLimit > 999999)
		return false;
		
	_limit = newLimit;
	return true;
}

bool	Channel::setTopic(const std::string& topic)
{
	if (topic == ":")
		_topic = "";
	else
		_topic = topic;
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

bool Channel::renameClient(const std::string &oldNick, const std::string &newNick)
{
	std::map<std::string, Client *>::iterator it = _clientChannelList.find(oldNick);
	if (it == _clientChannelList.end())
		return false;
	Client *client = it->second;
	_clientChannelList.erase(it);
	_clientChannelList[newNick] = client;

	if (_operators.erase(oldNick))
		_operators[newNick] = client;

	return true;
}
