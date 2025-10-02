#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "Ircserv.hpp"

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
		std::string			_channelKey;
		std::map<std::string, Client*>	_operators;
		std::map<std::string, Client*>	_clientChannelList;
		size_t					_limit;
		bool				_inviteOnly;
		bool				_topicRestriction;

		Channel();

	public:
		Channel(const std::string& name);
		~Channel();

		std::string	getName() const;
		bool		isInviteOnly() const;
		bool		isOperator(const Client& client) const;
		bool		isOperator(const std::string& nick) const;
		bool		isClient(const Client& client) const;
		bool		isClient(const std::string& nick) const;
		bool		isFull() const;

		bool		addClient(const Client& client, bool isOperator);
		bool		removeClient(const std::string& username, const std::string& msg);
		bool		changeKey(const std::string& key);
		bool		makeOperator(const std::string& nick);
		bool		inviteOnly();
		bool		topicRestriction();
		void		sendMessage(const std::string& msg, const std::string& prefix) const;
};

#endif