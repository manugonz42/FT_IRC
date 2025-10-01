#include "Ircserv.hpp"

struct Numeric
{
	std::string	numeric;
	std::string	description;
};

static Numeric makeNumeric(const std::string &numeric, const std::string &description)
{
    Numeric code;
    code.numeric = numeric;
    code.description = description;
    return code;
}

static	void fillErrors(std::map<int, Numeric> &errors)
{
	errors[401] = makeNumeric("401 ", " :No such nick/channel");
	errors[404] = makeNumeric("404 ", " :Cannot send to channel");
	errors[409] = makeNumeric("409 ", " :No origin specified");
	errors[411] = makeNumeric("411 ", " :No recipient given");
	errors[412] = makeNumeric("412 ", " :No text to send");
	errors[421] = makeNumeric("421 ", " :Unknown command");
	errors[431] = makeNumeric("431 ", " :No nickname given");
	errors[432] = makeNumeric("432 ", " :Erroneous nickname");
	errors[433] = makeNumeric("433 ", " :Nickname is already in use");
	errors[441] = makeNumeric("441 ", " :They aren't on that channel");
	errors[442] = makeNumeric("442 ", " :You're not on that channel");
	errors[443] = makeNumeric("443 ", " :is already on channel");
	errors[461] = makeNumeric("461 ", " :Not enough parameters");
	errors[462] = makeNumeric("462 ", " :You may not reregister");
	errors[471] = makeNumeric("471 ", " :Cannot join channel (+l)");
	errors[473] = makeNumeric("473 ", " :Cannot join channel (+i)");
	errors[474] = makeNumeric("474 ", " :Cannot join channel (+b)");
	errors[475] = makeNumeric("475 ", " :Cannot join channel (+k)");
	errors[482] = makeNumeric("482 ", " :You're not channel operator");
	errors[491] = makeNumeric("491 ", " :No O-lines for your host");
	errors[501] = makeNumeric("501 ", " :Unknown MODE flag");
	errors[502] = makeNumeric("502 ", " :Cannot change mode for other users");
}

int	sendNumeric(Client *client, int numeric, const std::string &target)
{
	std::map<int, Numeric> errors;
	fillErrors(errors);
	std::map<int, Numeric>::iterator it = errors.find(numeric);
	if (it != errors.end())
	{
		const Numeric &info = it->second;
		if (!::sendMessage(PREFIX, client->getFd(), info.numeric + client->getField("NICK") + info.description))
			return (0);
		return (1);
	}
	//error de find
}