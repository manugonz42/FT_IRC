#include "Ircserv.hpp"
#include <map>

struct Error
{
	std::string	numeric;
	std::string	description;
};

static Error makeError(const std::string &numeric, const std::string &description)
{
    Error err;
    err.numeric = numeric;
    err.description = description;
    return err;
}

void fillErrors(std::map<int, Error> &errors)
{
	errors[401] = makeError("401 ", ":No such nick/channel");
	errors[404] = makeError("404 ", ":Cannot send to channel");
	errors[409] = makeError("409 ", ":No origin specified");
	errors[411] = makeError("411 ", ":No recipient given");
	errors[412] = makeError("412 ", ":No text to send");
	errors[421] = makeError("421 ", ":Unknown command");
	errors[431] = makeError("431 ", ":No nickname given");
	errors[432] = makeError("432 ", ":Erroneous nickname");
	errors[433] = makeError("433 ", ":Nickname is already in use");
	errors[441] = makeError("441 ", ":They aren't on that channel");
	errors[442] = makeError("442 ", ":You're not on that channel");
	errors[443] = makeError("443 ", ":is already on channel");
	errors[461] = makeError("461 ", ":Not enough parameters");
	errors[462] = makeError("462 ", ":You may not reregister");
	errors[471] = makeError("471 ", ":Cannot join channel (+l)");
	errors[473] = makeError("473 ", ":Cannot join channel (+i)");
	errors[474] = makeError("474 ", ":Cannot join channel (+b)");
	errors[475] = makeError("475 ", ":Cannot join channel (+k)");
	errors[482] = makeError("482 ", ":You're not channel operator");
	errors[491] = makeError("491 ", ":No O-lines for your host");
	errors[501] = makeError("501 ", ":Unknown MODE flag");
	errors[502] = makeError("502 ", ":Cannot change mode for other users");
}

int	sendNumeric(Client *client, int numeric)
{
	std::map<int, Error> errors;
	fillErrors(errors);
	std::map<int, Error>::iterator it = errors.find(numeric);
	if (it != errors.end())
	{
		const Error &info = it->second;
		sendMessage(client->getFd(), info.numeric + client->getField("NICK") + info.description);
	}
}