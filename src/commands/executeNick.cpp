#include "Ircserv.hpp"


// Función para verificar si un nick ya está en uso
bool isSpecial(char c) 
{ 
    return c == '[' || c == ']' || c == '\\' || c == '`' || 
           c == '-' || c == '^' || c == '{' || c == '}' || c == '_'; 
}

// Función para validar el formato del nick
bool isValidNickname(const std::string &nick)
{
    if (nick.empty() || nick.length() > 9)
        return false;
    
    // Primer carácter: letra o especial
    if (!std::isalpha(nick[0]) && !isSpecial(nick[0]))
        return false;
    
    // Resto: letra, dígito, especial
    for (size_t i = 1; i < nick.length(); i++)
    {
        char c = nick[i];
        if (!std::isalnum(c) && !isSpecial(c))
            return false;
    }
    
    return true;
}

bool isValidNickCommand(Client *client, const ParsedCommand &cmd)
{
    if (cmd.params.empty())
    {
        sendNumeric(client, 431, ":No nickname given");
        return false;
    }
    if (cmd.params.size() < 2)
    {
        sendNumeric(client, 431, ":No nickname given");
        return false;
    }
    if (cmd.params.size() > 2) {
        sendNumeric(client, 461, ":Too many parameters");
        return false;
    }
    return true;
}

bool Server::executeNick(Client *client, const ParsedCommand &cmd)
{
    // 1. Validar cantidad de parámetros
    if (!isValidNickCommand(client, cmd))
        return false;
    
    // 2. Validar formato del nick
    if (!isValidNickname(cmd.params[1]))
    {
        sendNumeric(client, 432, cmd.params[1]);
        return false;
    }
    // 3. Verificar si ya está en uso
    std::string upperNick = strToUpper(cmd.params[1]);

    std::map<std::string, Client *>::iterator it = _clientMap.find(upperNick);
    if (it != _clientMap.end())
    {
        if (client->getLoginStatus() >= REGISTERED)
        {
            if (it->second == client)
            {
                client->setField("NICK", cmd.params[1]);
                return true; // Mismo cliente, mismo nick, ignorar
            }
            else
            {
                // No puede cambiar a un nick ya en uso
                sendNumeric(client, 433, cmd.params[1]);
                return false;
            }
        }
        else
        {
            // Nick usado durante login
            // Decidir que hacer--
            sendNumeric(client, 433, cmd.params[1]);
            return true;
        }
        
        return true;
    }
    
    // 4. Durante LOGIN
    if (client->getLoginStatus() < REGISTERED)
    {
        client->setField("NICK", cmd.params[1]);
        // Añadir al mapa provisionalmente, para evitar race condition en caso de recibir otro login con el mismo NICK antes de recibir el user de este.
        _clientMap[upperNick] = client;
        if (client->getLoginStatus() == USER_SENT)
        {
            client->setLoginStatus(REGISTERED);
            sendWelcome(client);
            return true;
        }
        client->setLoginStatus(NICK_SENT);
        
        return true;
    }
    
    // 5. Cambio de nick después del login
    else
    {
        std::string oldNick = client->getField("NICK");
        client->setField("NICK", cmd.params[1]);

        // Actualizar el mapa de clientes
        std::string oldUpperNick = strToUpper(oldNick);
        _clientMap.erase(oldUpperNick);     
        _clientMap[upperNick] = client;     
        
        // Notificar al cliente del cambio
        ::sendMessage(PREFIX, client->getFd(), ":" + oldNick + " NICK " + cmd.params[1] + "\r\n");
        
        // Notificar a todos los canales del cambio
        //notifyNickChange(client, oldNick, cmd.params[1]);
        return true;
    }
}