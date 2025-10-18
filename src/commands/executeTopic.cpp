#include "Ircserv.hpp"

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

	std::string	topicMsg = "";

	if (cmd.params.size() == 2)
	{
		std::string topic = channel->getTopic();
		topicMsg += channel->getName() + " :";

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
		topicMsg += "TOPIC " + cmd.params[1] + " " + channel->getTopic();
		if (!channel->sendMessage(NULL, topicMsg, client->getField("PREFIX")))
			return false;
	}
	return true;
}