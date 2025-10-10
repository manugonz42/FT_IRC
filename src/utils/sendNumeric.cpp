#include "Ircserv.hpp"

// static	void fillErrors(std::map<int, Numeric> &errors, const std::string &target)
// {
// 	errors[401] = makeNumeric("401 ", target);
// 	errors[403] = makeNumeric("403 ", "No such channel");
// 	errors[404] = makeNumeric("404 ", " :Cannot send to channel");
// 	errors[409] = makeNumeric("409 ", " :No origin specified");
// 	errors[411] = makeNumeric("411 ", " :");
// 	errors[412] = makeNumeric("412 ", " :");
// 	errors[421] = makeNumeric("421 ", " :Unknown command");
// 	errors[431] = makeNumeric("431 ", " :No nickname given");
// 	errors[432] = makeNumeric("432 ", " :Erroneous nickname");
// 	errors[433] = makeNumeric("433 ", " :Nickname is already in use");
// 	errors[441] = makeNumeric("441 ", " :They aren't on that channel");
// 	errors[442] = makeNumeric("442 ", " :You're not on that channel");
// 	errors[443] = makeNumeric("443 ", " :is already on channel");
// 	errors[461] = makeNumeric("461 ", " :Not enough parameters");
// 	errors[462] = makeNumeric("462 ", " :You may not reregister");
// 	errors[464] = makeNumeric("464 ", " :Password incorrect");
// 	errors[471] = makeNumeric("471 ", " :Cannot join channel (+l)");
// 	errors[473] = makeNumeric("473 ", " :Cannot join channel (+i)");
// 	errors[474] = makeNumeric("474 ", " :Cannot join channel (+b)");
// 	errors[475] = makeNumeric("475 ", " :Cannot join channel (+k)");
// 	errors[482] = makeNumeric("482 ", " :You're not channel operator");
// 	errors[491] = makeNumeric("491 ", " :No O-lines for your host");
// 	errors[501] = makeNumeric("501 ", " :Unknown MODE flag");
// 	errors[502] = makeNumeric("502 ", " :Cannot change mode for other users");
// }

// PRIVMSG #channel

int	sendNumeric(Client *client, int numeric, const std::string &target)
{
	switch (numeric)
	{
		case 401:
			if (!::sendMessage(PREFIX, client->getFd(), "401 " + client->getField("NICK") + " :" + target))
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
		case 451:
			if (!::sendMessage(PREFIX, client->getFd(), "451 " + client->getField("NICK") + " :You have not registered"))
				return (0);
			break;
		case 461:
			if (!::sendMessage(PREFIX, client->getFd(), "461 " + client->getField("NICK") + target + " :Not enough parameters"))
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
		default:
			break;
	}
	return (1);
}