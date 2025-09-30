#include "Ircserv.hpp"
// Función para verificar si un nick ya está en uso
bool isSpecial(char c) 
{ 
    return c == '[' || c == ']' || c == '\\' || c == '`' || 
           c == '_' || c == '^' || c == '{' || c == '|' || c == '}'; 
}

// Función para validar el formato del nick
bool isValidNickname(const std::string &nick)
{
    if (nick.empty() || nick.length() > 9)
        return false;
    
    // Primer carácter: letra o especial
    char first = nick[0];
    if (!std::isalpha(first) && !isSpecial(first))
        return false;
    
    // Resto: letra, dígito, especial o guión
    for (size_t i = 1; i < nick.length(); i++)
    {
        char c = nick[i];
        if (!std::isalnum(c) && !isSpecial(c) && c != '-')
            return false;
    }
    
    return true;
}

bool Server::executeNick(Client *client, const ParsedCommand &cmd)
{
    // 1. Validar parámetros
    if (cmd.params.empty())
    {
        // error 461
        return false;
    }
    
    // 2. Validar formato del nick
    if (!isValidNickname(cmd.params[1]))
    {
        // error 432
        return false;
    }
    
    // 3. Verificar si ya está en uso
    if (isNicknameInUse(cmd.params[1]))
    {
        if (client->getLoginStatus() < REGISTERED)
        {
            // Durante login
            // error 433
        }
        else
        {
            // Cliente ya registrado
            // error 433
        }
        return false;
    }
    
    // 4. Durante LOGIN
    if (client->getLoginStatus() < REGISTERED)
    {
        client->setField("NICK", cmd.params[1]);
        client->setLoginStatus(NICK_SENT);
        // Verificar si puede completar registro
        if (canCompleteRegistration(client))
            completeRegistration(client);
        return true;
    }
    
    // 5. Cambio de nick después del login
    else
    {
        std::string oldNick = client->getField("NICK");
        client->setField("NICK", cmd.params[0]);
        
        // Notificar al cliente del cambio
        sendMessage(client->getFd(), ":" + oldNick + " NICK " + cmd.params[0] + "\r\n");
        
        // Notificar a todos los canales donde está el usuario
        notifyNickChange(client, oldNick, cmd.params[0]);
        return true;
    }
}