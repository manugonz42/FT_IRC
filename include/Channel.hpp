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
	public:
		Channel();
		~Channel();
};

#endif