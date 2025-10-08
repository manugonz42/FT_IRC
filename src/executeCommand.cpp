#include "Ircserv.hpp"

// bool	Server::executeCommand(Client *client, const ParsedCommand &cmd)
// {
// 	for (size_t i = 0; i < cmd.params.size(); ++i)
// 		std::cout << "  Param[" << i << "]: '" << cmd.params[i] << "'" << std::endl;
// 	std::map<std::string, bool (Server::*)(Client *client, const ParsedCommand &cmd)>::iterator	it;
// 	it = _commandMap.find(cmd.params[0]);
// 	if (it != _commandMap.end())
// 		return (it->second(client, cmd));
// 	return (true);
// }