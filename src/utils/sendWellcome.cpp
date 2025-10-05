#include "Ircserv.hpp"

bool Server::sendWelcome(Client *client)
{
        ::sendMessage(PREFIX, client->getFd(),
        std::string(BOLD) + BRIGHT_GREEN +
        "001 " + client->getField("NICK") + " :Welcome to " +
        BRIGHT_CYAN + BOLD + "the.best.of.the.world.irc.server" +
        RESET + BRIGHT_GREEN + ", " + client->getField("NICK") +
        "! Connection established successfully." +
        RESET);

        ::sendMessage(PREFIX, client->getFd(),
        std::string(BOLD) + BRIGHT_GREEN +
        "002 " + client->getField("NICK") + " :Your host is " +
        BRIGHT_CYAN + BOLD + "ft_irc" +
        RESET + BRIGHT_GREEN + ", running version " +
        WHITE + BOLD + "1.0" +
        RESET + BRIGHT_GREEN + "." +
        RESET);

        ::sendMessage(PREFIX, client->getFd(),
        std::string(BOLD) + BRIGHT_GREEN +
        "003 " + client->getField("NICK") + " :This server was built for " +
        BRIGHT_CYAN + BOLD + "clear communication" +
        RESET + BRIGHT_GREEN + " and reliable connections." +
        RESET);

        std::cout << GREEN << "[SUCCESS] " << RESET
        << "Client " << BOLD << BLUE << client->getFd() << RESET
        << " fully registered as " << BOLD << MAGENTA << client->getField("NICK") << RESET
        << BRIGHT_GREEN << " ✅" << RESET << std::endl;

        return true;
}