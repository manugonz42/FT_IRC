#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

static bool	isValid(const std::string& channel)
{
	return (!channel.empty() && channel[0] == '#');
}

std::vector<std::string> parseChannels(const std::string& channelParam) {
	std::vector<std::string> channels;
		
	if (channelParam.empty())
		return channels;
		
	std::string current = "";
	for (size_t i = 0; i < channelParam.length(); ++i) {
		char c = channelParam[i];
		
		if (c == ',') {
			if (isValid(current)) {
				channels.push_back(current);
				current = "";
			}
		} else 
			current += c;
		
	}

	if (!current.empty())
		channels.push_back(current);
		
	return channels;
}