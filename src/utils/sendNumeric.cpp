#include "Ircserv.hpp"

int	sendNumeric(Client *client, int numeric, const std::string &target)
{
	switch (numeric)
	{
		case 401:
			if (!::sendMessage(PREFIX, client->getFd(), "401 " + client->getField("NICK") + " :" + target))
				return (0);
			break;
		case 402:
			if (!::sendMessage(PREFIX, client->getFd(), "402 " + client->getField("NICK") + " :No such server"))
				return (0);
			break;
		case 403:
			if (!::sendMessage(PREFIX, client->getFd(), "403 " + client->getField("NICK") + " :" + target))
				return (0);
			break;
		case 404:
			if (!::sendMessage(PREFIX, client->getFd(), "404 " + client->getField("NICK") + " :" + target))
				return (0);
			break;
		case 411:
			if (!::sendMessage(PREFIX, client->getFd(), "411 " + client->getField("NICK") + " :No recipient given"))
				return (0);
			break;
		case 412:
			if (!::sendMessage(PREFIX, client->getFd(), "412 " + client->getField("NICK") + " :No text to send"))
				return (0);
			break;
		case 431:
			if (!::sendMessage(PREFIX, client->getFd(), "431 " + client->getField("NICK") + " :No nickname given"))
				return (0);
			break;
		case 432:
			if (!::sendMessage(PREFIX, client->getFd(), "432 " + client->getField("NICK") + " :" + target + " :Erroneous nickname"))
				return (0);
			break;
		case 433:
			if (!::sendMessage(PREFIX, client->getFd(), "433 " + client->getField("NICK") + " " + target + " :Nickname is already in use"))
				return (0);
			break;
		case 441:
			if (!::sendMessage(PREFIX, client->getFd(), "441 " + client->getField("NICK") + " " + target + " :They aren't on that channel"))
				return (0);
			break;
		case 442:
			if (!::sendMessage(PREFIX, client->getFd(), "442 " + client->getField("NICK") + " " + target + " :You're not on that channel"))
				return (0);
			break;
		case 451:
			if (!::sendMessage(PREFIX, client->getFd(), "451 " + client->getField("NICK") + " :You have not registered"))
				return (0);
			break;
		case 461:
			if (!::sendMessage(PREFIX, client->getFd(), "461 " + client->getField("NICK") + " :" + YELLOW + target + RESET))
				return (0);
			break;
		case 462:
			if (!::sendMessage(PREFIX, client->getFd(), "462 " + client->getField("NICK") + " :You may not reregister"))
				return (0);
			break;
		case 464:
			if (!::sendMessage(PREFIX, client->getFd(), "464 " + client->getField("NICK") + " :" + RED + "Password Incorrect" + RESET))
				return (0);
			break;
		case 482:
			if (!::sendMessage(PREFIX, client->getFd(), "482 " + client->getField("NICK") + " :" + target + " :You're not channel operator"))
				return (0);
			break;
		case 351:
			if (!::sendMessage(PREFIX, client->getFd(), "351 " + client->getField("NICK") + " " + target))
				return (0);
			break;
		default:
			break;
	}
	return (1);
}