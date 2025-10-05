#include "Ircserv.hpp"
/*
bool    isValidUsername(const std::string &username)
{
    if (!isValidLength(username, 1, 12)) // RFC sin limite max, decidir si ponerle
        return false;
    
    for (size_t i = 0; i < username.length(); i++)
    {
        char c = username[i];
        if (c <= 32 || c == 127 || c == '@') // Caracteres no permitidos
            return false;
    }
    
    return true;
}

bool    executeUser(Client *client, const ParsedCommand &cmd)
{
    // Validar estado del cliente
    if (client->getLoginStatus() < NICK_SENT)
    {
        // Error: debe enviar NICK válido primero
        // error 451
        return true;
    }
    if (client->getLoginStatus() >= REGISTERED)
    {
        // Error 462: You may not reregister
        return true;
    }

    // 1. Validar cantidad de parámetros
    if (cmd.params.size() < 5)
    {
        // Error 461: Not enough parameters
        return true;
    }

    // Validar formato de los parámetros
    
    
    std::string username = cmd.params[1];
    std::string host = cmd.params[2];
    std::string realname = cmd.params[4];
    
    // Completar registro
    client->setField("USER", username);
    client->setField("HOST", host); // usar DNS reverse?
    client->setField("REAL", realname);
    client->setLoginStatus(REGISTERED);
    
    // Enviar welcome messages
    ::sendMessage(PREFIX, client->getFd(), "001 " + nick + " :Welcome to the IRC Network " + nick + "!");
    ::sendMessage(PREFIX, client->getFd(), "002 " + nick + " :Your host is server, running version 1.0");
    ::sendMessage(PREFIX, client->getFd(), "004 " + nick + " server 1.0");
    
    std::cout << "Client " << client->getFd() << " fully registered as " << nick << std::endl;
    
    return true;
}*/