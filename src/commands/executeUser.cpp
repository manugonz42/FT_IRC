#include "Ircserv.hpp"

std::string buildRealname(const ParsedCommand &cmd)
{
    // Si solo hay 5 parámetros quitar ':' si existe
    if (cmd.params.size() == 5)
	{
        if (cmd.params[4][0] == ':')
            return cmd.params[4].substr(1);
        else
            return cmd.params[4];
   }
    
    // Concatenar parámetros 4 en adelante en un solo string
    std::string realname = cmd.params[4];
    for (size_t i = 5; i < cmd.params.size(); i++)
        realname += " " + cmd.params[i];
    return realname;
}

bool    isValidRealname(const std::string &realname)
{
    for (size_t i = 0; i < realname.length(); i++)
	{
        char c = realname[i];
        if (c == '\0' || c == '\r' || c == '\n')
            return false;
    }
    return true; 
}

bool    isValidUsername(const std::string &username)
{
	// RFC sin limite max, decidir si ponerle
    if (!isValidLength(username, 1, 510))
        return false;

    for (size_t i = 0; i < username.length(); i++)
    {
        char c = username[i];
        if (c <= 32 || c == 127 || c == '@')
            return false;
    }
 
    return true;
}

bool    Server::executeUser(Client *client, const ParsedCommand &cmd)
{
    if (client->getLoginStatus() >= USER_SENT)
		return (sendNumeric(client, 462, ""));

    std::string nick = client->getField("NICK");

    if (cmd.params.size() < 5)
        return (sendNumeric(client, 461, "USER :Not enough parameters"));

    if (!isValidUsername(cmd.params[1]))
        return (sendNumeric(client, 461, "USER :Invalid username format"));
    if (!isValidRealname(cmd.params[4]))
        return (sendNumeric(client, 461, "USER :Invalid realname format"));
    
    std::string username = cmd.params[1];
    std::string host = cmd.params[2];
    std::string realname = buildRealname(cmd);
    
    client->setField("USER", username);
    client->setField("HOST", host); // usar DNS reverse?
    client->setField("REAL", realname);
    if (client->getLoginStatus() == NICK_SENT)
    {
        client->setLoginStatus(REGISTERED);
        if (!sendWelcome(client))
			return false;
    }
    else
        client->setLoginStatus(USER_SENT);
    
    return true;
}