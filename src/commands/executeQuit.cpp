#include "Ircserv.hpp"

bool	Server::executeQuit(Client *client, const ParsedCommand &cmd)
{
	std::string reasson = ":Quit for no reasson";
	if (client->getLoginStatus() < REGISTERED)
		return (false);
	if (cmd.params.size() > 2)
		return (sendNumeric(client, 461, " QUIT :Too many parameters"));
	if (cmd.params.size() == 2)
		reasson = cmd.params[1];
	if (reasson == ":leaving")
		reasson = ":Client Quit";
	notifyToAllChannels(client->getField("PREFIX"), client, "QUIT " + reasson);
	removeClientFromChannels(client);
	return (false);
}
