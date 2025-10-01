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
	_limit = 10;
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

//////////////////////////////////////////////////////////////////
//																//
//							Member Functions					//
//																//
//////////////////////////////////////////////////////////////////

bool	Channel::addClient(const Client& client, bool makeOperator)
{
    std::string nick = client.getField("NICK");    
    if (nick.empty()) {
        std::cout << "ERROR: Client nick is empty!" << std::endl;
        return false;
    }
    
    _clientChannelList[nick] = const_cast<Client *>(&client);
    if (makeOperator)
		_operators[nick] = const_cast<Client *>(&client);

    std::string channelName = _name;
    
    // 1. Confirmar JOIN al cliente
    std::string joinMsg = nick + "!user@host JOIN " + channelName;
    ::sendMessage(":server " ,client.getFd(), joinMsg);
    
    
    // 2. Enviar lista de usuarios (RPL_NAMREPLY)
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
    
    std::string namesMsg = "353 " + nick + " = " + channelName + " :" + userList;
    ::sendMessage(":server " , client.getFd(), namesMsg);
    
    // 3. Fin de la lista (RPL_ENDOFNAMES)
    std::string endMsg = "366 " + nick + " " + channelName + " :End of /NAMES list";
    ::sendMessage(":server ", client.getFd(), endMsg);


	std::map<std::string, Client *>::iterator	it2 = _clientChannelList.begin();
	for (;it2 != _clientChannelList.end(); ++it2)
		std::cout << "Cliente: " << it2->first << std::endl;


    return true;
}

bool	Channel::removeClient(const std::string& nick, const std::string& msg)
{
	std::map<std::string, Client *>::iterator	it = _clientChannelList.find(nick);
	if (it == _clientChannelList.end())
		return false;
	if (msg != "")
		::sendMessage(":server ", it->second->getFd(), msg);
	_clientChannelList.erase(it);
	return true;
}

bool	Channel::changeKey(const std::string& key)
{
	this->_channelKey = key;
	return true;
}

bool	Channel::makeOperator(const std::string& nick)
{
	std::map<std::string, Client *>::iterator	it = _clientChannelList.find(nick);
	if (it == _clientChannelList.end())
		return false;
	this->_operators[nick] = it->second;
	return true;
}

bool	Channel::inviteOnly()
{
	_inviteOnly = !_inviteOnly;
	return true;
}

bool	Channel::topicRestriction()
{
	_topicRestriction = !_topicRestriction;
	return true;
}

void	Channel::sendMessage(const std::string& msg) const
{
	std::map<std::string, Client *>::const_iterator	it = _clientChannelList.begin();
	std::map<std::string, Client *>::const_iterator	end = _clientChannelList.end();
	for (; it != end; ++it)
	{
//		std::cout << "Cliente: " << it->first << std::endl;
		//::sendMessage(it->second->getFd(), msg);
		std::string	wire = ":jimmy " + msg + "\r\n";
		if (::send(it->second->getFd(), wire.c_str(), wire.size(), 0) == -1)
		{
			std::cerr << "Error: send failed" << std::endl;
			return;
		}
	}
}

