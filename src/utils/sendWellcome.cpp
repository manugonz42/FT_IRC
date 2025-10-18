#include "Ircserv.hpp"

bool Server::sendWelcome(Client *client)
{
		if (!::sendMessage(PREFIX, client->getFd(), "001 " + client->getField("NICK") + " :" +
		std::string(BOLD) + BRIGHT_GREEN + "Welcome to " + BRIGHT_CYAN + BOLD + "the.best.of.the.world.irc.server" +
		RESET + BRIGHT_GREEN + ", " + client->getField("NICK") +
		"! Connection established successfully." + RESET))
			return false;

		if (!::sendMessage(PREFIX, client->getFd(), "002 " + client->getField("NICK") + " :" +
		std::string(BOLD) + BRIGHT_GREEN + "Your host is " + BRIGHT_CYAN + BOLD + "ft_irc" +
		RESET + BRIGHT_GREEN + ", running version " +
		WHITE + BOLD + "1.0" +
		RESET + BRIGHT_GREEN + "." + RESET))
			return false;

		if (!::sendMessage(PREFIX, client->getFd(), "003 " + client->getField("NICK") + " :" +
		std::string(BOLD) + BRIGHT_GREEN + "This server was built for " +
		BRIGHT_CYAN + BOLD + "clear communication" +
		RESET + BRIGHT_GREEN + " and reliable connections." + RESET))
			return false;

		std::cout << GREEN << "[SUCCESS] " << RESET
		<< "Client " << BOLD << BLUE << client->getFd() << RESET
		<< " fully registered as " << BOLD << MAGENTA << client->getField("NICK") << RESET
		<< BRIGHT_GREEN << " ✅" << RESET << std::endl;

		client->setPrefix();

		return true;
}