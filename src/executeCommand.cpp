#include "Ircserv.hpp"

bool	Server::executeCommand(Client *client, const ParsedCommand &cmd)
{
	for (size_t i = 0; i < cmd.params.size(); ++i)
		std::cout << "  Param[" << i << "]: '" << cmd.params[i] << "'" << std::endl;
	std::map<std::string, bool (Server::*)(Client *client, const ParsedCommand &cmd)>::iterator	it;
	it = _commandMap.find(cmd.command);
	if (it != _commandMap.end())
	{
		if (cmd.command == "JOIN" && cmd.params.size() > 1 && cmd.params[1] == ":" && client->getLoginStatus() != REGISTERED)
			return (true);
		if (it->first != "PASS" && it->first != "CAP" && client->getLoginStatus() < PASS_SENT)
		{
			sendNumeric(client, 464, "");
			return (false);
		}
		else if (it->first != "NICK" && it->first != "USER" && it->first != "PASS" && it->first != "CAP" &&  client->getLoginStatus() != REGISTERED)
		{
			sendNumeric(client, 451, "");
			return (true);
		}
		return ((this->*(it->second))(client, cmd));
	}
	return (true);
}