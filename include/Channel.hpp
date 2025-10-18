#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Ircserv.hpp"
#include <sstream>
#include <set>

class Channel
{
	private:
		std::string						_name;
		std::string						_topic;
		std::string						_channelKey;
		std::map<std::string, Client*>	_operators;
		std::set<std::string>			_whiteList;
		std::set<std::string>			_blackList;
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
		bool		isLimited() const;
		bool		isOperator(const Client& client) const;
		bool		isOperator(const std::string& nick) const;
		bool		isClient(const Client& client) const;
		bool		isClient(const std::string& nick) const;
		bool		isInvited(const Client& client) const;
		bool		isBanned(const Client& client) const;
		bool		isFull() const;
		bool		isEmpty() const;
		bool		hasKey() const;
		std::string	getClients() const;
		std::string	getModes() const;
		std::string getParameters() const;
		std::string	getTopic() const;

		bool		addClient(const Client& client, bool makeOperator);
		bool		inviteClient(Client* client);
		bool		bannClient(const std::string& nick);
		bool		removeClient(const std::string& nick);
		bool		changeKey(const std::string& key);
		bool		introduceKey(const std::string& key) const;
		bool		changeLimit(const std::string& limit);
		bool		setTopic(const std::string& topic);
		bool		makeOperator(const std::string& nick);
		bool		removeOperator(const std::string& nick);
		bool		inviteOnly(bool inviteOnly);
		bool		topicRestriction(bool topicRestriction);
		bool		sendMessage(Client *client, const std::string& msg, const std::string& prefix) const;
		bool		renameClient(const std::string &oldNick, const std::string &newNick);
	};

#endif