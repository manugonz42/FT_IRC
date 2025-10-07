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
    if (client->getLoginStatus() >= USER_SENT)
    {
        // Error 462: You may not reregister
        sendNumeric(client, 462, ":Unauthorized command (already registered)");
        return true;
    }

    // Para el mensaje
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
    
    std::string username = cmd.params[2];	// Es el 2!!!
    std::string host = cmd.params[3];		// Es el 3!!!
    std::string realname = buildRealname(cmd);
    
    // Completar registro
    client->setField("USER", username);
    client->setField("HOST", host); // usar DNS reverse?
    client->setField("REAL", realname);
    if (client->getLoginStatus() == NICK_SENT)
    {
        client->setLoginStatus(REGISTERED);
        sendWelcome(client);
    }
    else
        client->setLoginStatus(USER_SENT);
    
    return true;
}