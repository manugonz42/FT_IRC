#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Ircserv.hpp"
#include <sstream>

class Channel
{
	private:
		std::string						_name;
		std::string						_topic;
		std::string						_channelKey;
		std::map<std::string, Client*>	_operators;
		std::map<std::string, Client*>	_clientChannelList;
		size_t							_limit;
		bool							_inviteOnly;
		bool							_topicRestriction;

		Channel();

	public:
		Channel(const std::string& name);
		~Channel();

		std::string	getName() const;
		bool		isInviteOnly() const;
		bool		isTopicRestricted() const;
		bool		isKeyProtected() const;
		bool		isLimited() const;
		bool		isOperator(const Client& client) const;
		bool		isOperator(const std::string& nick) const;
		bool		isClient(const Client& client) const;
		bool		isClient(const std::string& nick) const;
		bool		isFull() const;
		bool		hasPass() const;
		std::string	getClients() const;
		std::string	getModes() const;
		std::string getParameters() const;

		bool		addClient(const Client& client, bool makeOperator);
		bool		removeClient(const std::string& nick);
		bool		changeKey(const std::string& key);
		bool		changeLimit(const std::string& limit);
		bool		makeOperator(const std::string& nick);
		bool		removeOperator(const std::string& nick);
		bool		inviteOnly(bool inviteOnly);
		bool		topicRestriction(bool topicRestriction);
		bool		sendMessage(Client *client, const std::string& msg, const std::string& prefix) const;
	};

#endif