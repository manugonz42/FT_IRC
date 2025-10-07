#include "Ircserv.hpp"

bool	Server::executePass(Client *client, const ParsedCommand &cmd)
{
	if (client->getLoginStatus() >= PASS_SENT)
		return (sendNumeric(client, 462, ""));
	if (cmd.params.size() >= 2)
	{
		std::string password = cmd.params[1];
		if (password == _password)
		{
			client->Authenticate();
			client->setLoginStatus(PASS_SENT);
			std::cout << "Client " << client->getFd() << " authenticated" << std::endl;
		}
		else
		{
			sendNumeric(client, 464, "");
			return false;
		}
	}
	else
	{
		return (sendNumeric(client, 461, ""));
	}
	return (true);
}