#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

static bool	isValid(const ParsedCommand& cmd)
{
	return (cmd.params.size() >= 2 && cmd.params.size() <= 3 && cmd.params[1][0] == '#');
}


bool	Server::executeTopic(Client *client, const ParsedCommand &cmd)
{
	if (!isValid(cmd))
	{
		if (!sendNumeric(client, 461, "TOPIC :Not enough parameters"))
			return false;
		return true;
	}

	std::map<std::string,Channel*>::iterator itChannel = _channelMap.find(cmd.params[1]);
	if (itChannel == _channelMap.end())
	{
		if (!sendNumeric(client, 403, cmd.params[1]))
			return false;
		return true;
	}

	Channel*	channel = itChannel->second;

	if (!channel->isClient(*client))
	{
		if (!sendNumeric(client, 442, cmd.params[1]))
			return false;
		return true;
	}

	if (cmd.params.size() == 2)
	{
		std::string topic = channel->getTopic();
		std::string topicMsg = channel->getName() + " :";

		if (topic.empty())
		{
			topicMsg += "No topic is set";
			if (!sendNumeric(client, 331, topicMsg))
				return false;
		}
		else
		{
			topicMsg += topic;
			if (!sendNumeric(client, 332, topicMsg))
				return false;
		}
		return true;
	}
	else
	{
		if (channel->isTopicRestricted() && !channel->isOperator(*client))
		{
			if (!sendNumeric(client, 482, cmd.params[1]))
				return false;
			return true;
		}
		channel->setTopic(cmd.params[2]);
	}
	return true;
}