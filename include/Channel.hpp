#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Ircserv.hpp"

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::string			_channelKey;
		std::map<std::string, Client>	_operators;
		std::map<std::string, Client>	_clientChannelList;
		int					_limit;
		bool				_inviteOnly;
		bool				_topicRestriction;

		Channel();

	public:
		Channel(const std::string& name);
		~Channel();

		std::string	getName() const;
		bool		isInviteOnly() const;
		bool		isOperator(const Client& client) const;

		bool		addClient(const Client& client);
		bool		removeClient(const std::string& username);
		bool		changeKey(const std::string& key);
		bool		makeOperator();
		bool		inviteOnly();
		bool		topicRestriction();
};

#endif