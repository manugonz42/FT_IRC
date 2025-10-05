#include "Ircserv.hpp"

std::string buildRealname(const ParsedCommand &cmd) {
   
    // Si solo hay 5 parámetros quitar ':' si existe
    if (cmd.params.size() == 5) {
        if (cmd.params[4][0] == ':')
            return cmd.params[4].substr(1);
        else
            return cmd.params[4];
    }
    
    // Concatenar parámetros 4 en adelante en un solo string
    std::string realname = cmd.params[4];
    for (size_t i = 5; i < cmd.params.size(); i++) {
        realname += " " + cmd.params[i];
    }
    return realname;
}

bool    isValidRealname(const std::string &realname) {
    for (size_t i = 0; i < realname.length(); i++) {
        char c = realname[i];
        if (c == '\0' || c == '\r' || c == '\n')
            return false;
    }
    return true; 
}

bool    isValidUsername(const std::string &username)
{
    if (!isValidLength(username, 1, 510)) // RFC sin limite max, decidir si ponerle
        return false;
    
    for (size_t i = 0; i < username.length(); i++)
    {
        char c = username[i];
        if (c <= 32 || c == 127 || c == '@') // Caracteres no permitidos
            return false;
    }
    
    return true;
}

bool    Server::executeUser(Client *client, const ParsedCommand &cmd)
{
    // Validar estado del cliente
    if (client->getLoginStatus() < NICK_SENT)
    {
        // Error: debe enviar NICK válido primero
        sendNumeric(client, 451, ":You have not registered");
        return true;
    }
    if (client->getLoginStatus() >= REGISTERED)
    {
        // Error 462: You may not reregister
        sendNumeric(client, 462, ":Unauthorized command (already registered)");
        return true;
    }

    // Obtener nick (ya validado por NICK_SENT)
    std::string nick = client->getField("NICK");

    // 1. Validar cantidad de parámetros
    if (cmd.params.size() < 5)
    {
        // Error 461: Not enough parameters
        sendNumeric(client, 461, "USER :Not enough parameters");
        return true;
    }

    // Validar formato de los parámetros
    if (!isValidUsername(cmd.params[1]))
    {
        // Error 461: Invalid username format
        sendNumeric(client, 461, "USER :Invalid username format");
        return true;
    }
    if (!isValidRealname(cmd.params[4]))
    {
        // Error 461: Invalid realname format
        sendNumeric(client, 461, "USER :Invalid realname format");
        return true;
    }
    
    std::string username = cmd.params[1];
    std::string host = cmd.params[2];
    std::string realname = buildRealname(cmd);
    
    // Completar registro
    client->setField("USER", username);
    client->setField("HOST", host); // usar DNS reverse?
    client->setField("REAL", realname);
    client->setLoginStatus(REGISTERED);
    
    // Enviar welcome messages con colores IRC
    ::sendMessage(PREFIX, client->getFd(),
    std::string(BOLD) + BRIGHT_GREEN +
    "001 " + nick + " :Welcome to " +
    BRIGHT_CYAN + BOLD + "the.best.of.the.world.irc.server" +
    RESET + BRIGHT_GREEN + ", " + nick +
    "! Connection established successfully." +
    RESET);

::sendMessage(PREFIX, client->getFd(),
    std::string(BOLD) + BRIGHT_GREEN +
    "002 " + nick + " :Your host is " +
    BRIGHT_CYAN + BOLD + "ft_irc" +
    RESET + BRIGHT_GREEN + ", running version " +
    WHITE + BOLD + "1.0" +
    RESET + BRIGHT_GREEN + "." +
    RESET);

::sendMessage(PREFIX, client->getFd(),
    std::string(BOLD) + BRIGHT_GREEN +
    "003 " + nick + " :This server was built for " +
    BRIGHT_CYAN + BOLD + "clear communication" +
    RESET + BRIGHT_GREEN + " and reliable connections." +
    RESET);

std::cout << GREEN << "[SUCCESS] " << RESET
          << "Client " << BOLD << BLUE << client->getFd() << RESET
          << " fully registered as " << BOLD << MAGENTA << nick << RESET
          << BRIGHT_GREEN << " ✅" << RESET << std::endl;

    
    return true;
}