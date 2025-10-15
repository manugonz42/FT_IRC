#include "Ircserv.hpp"

bool Server::executeVersion(Client *client, const ParsedCommand &cmd)
{
    // VERSION sin parámetros
    if (cmd.params.size() == 1)
    {
        sendNumeric(client, 351, std::string(SERVER_VERSION) + "." + DEBUGLEVEL + " " + _serverName + " :" + BLUE + COMMENTS + RESET);
        return true;
    }
    
    // VERSION <target>
    if (cmd.params.size() == 2)
    {
        std::string target = cmd.params[1];
        
        // Si el target es nuestro servidor, responder normalmente
        if (target == _serverName || target == "localhost" || target == "127.0.0.1")
        {

            sendNumeric(client, 351, std::string(SERVER_VERSION) + "." + DEBUGLEVEL + " " + _serverName + " :" + BLUE + COMMENTS + RESET);
            return true;
        }
        
        sendNumeric(client, 402, " :No such server");
        return false;
    }
    
    // Demasiados parámetros
    sendNumeric(client, 461, "VERSION :Not enough parameters");
    return false;
}