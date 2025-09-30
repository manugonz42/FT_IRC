#include "Ircserv.hpp"

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
	_topic = NULL;
	_channelKey = NULL;
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

bool		Channel::isOperator(const Client& client)
{
	for (Client c : _operators)
		if (c == client)
			return true;
	return false;
}

//////////////////////////////////////////////////////////////////
//																//
//							Member Functions					//
//																//
//////////////////////////////////////////////////////////////////

bool	Channel::addClient(const Client& client)
{
	_clientChannelList.push_back(client);
	return true;
}

bool	Channel::removeClient(const std::string& username)
{
	std::vector<Client>::iterator	it = _clientChannelList.begin();
	while (*it.getUsername() != username)
		it++;
	if (it == _clientChannelList.end())
		return false;
	_clientChannelList.erase(it);
	return true;
}

bool	Channel::changeKey(const std::string& key)
{
	this->_channelKey = key;
	return true;
}

bool	Channel::makeOperator(const std::string& username)
{
	std::vector<Client>::iterator	it = _clientChannelList.begin();
	while (*it.getUsername() != username)
		it++;
	if (it == _clientChannelList.end())
		return false;
	this->_operators.push_back(*it);
	return true;
}

bool	Channel::inviteOnly()
{
	_inviteOnly = !_inviteOnly;
}

bool	Channel::topicRestriction()
{
	_topicRestriction = !_topicRestriction;
}
