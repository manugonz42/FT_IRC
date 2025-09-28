#include "Ircserv.hpp"

void	Server::removeClient(size_t i)
{
	if (i >= _clientList.size())
		return;
	shutdown(_clientList[i]->getFd(), SHUT_WR);
	delete _clientList[i];
	_clientList.erase(_clientList.begin() + i);
}

void	Server::shutdownClients()
{
	size_t	i = 1;

	while (i < _pollFds.size())
	{
		size_t	client = i - 1;
		close(_pollFds[i].fd);
		_pollFds.erase(_pollFds.begin() + i);
		removeClient(client);
	}
}

void	Server::acceptNewClient()
{
	if (_pollFds[0].revents & POLLIN)
	{
		struct sockaddr_in	addr;
		socklen_t			len = sizeof(addr);
		int	clientFd = accept(_fd, (struct sockaddr *)&addr, &len);
		if (clientFd < 0)
			std::cerr << "Error: accept failed" << std::endl;
		else
		{
			Client *newClient = new Client(clientFd);
			_clientList.push_back(newClient);
			struct pollfd clientPoll;
			clientPoll.fd = newClient->getFd();
			clientPoll.events = POLLIN;
			clientPoll.revents = 0;
			_pollFds.push_back(clientPoll);
			
			std::cout << "Client connected: " << newClient->getFd() << std::endl;
		}
	}
}

void	Server::processClientsInput()
{
	char		buffer[512];
	std::string	line;
	size_t		i = 1;
	
	while (i < _pollFds.size())
	{
		size_t	client = i - 1;
		short	revents = _pollFds[i].revents;
		if (revents & (POLLERR | POLLHUP | POLLNVAL))
		revents |= POLLIN;
		if (revents & POLLIN)
		{
			int	bytes = recv(_pollFds[i].fd, buffer, sizeof(buffer) - 1, 0);
			if (bytes <= 0)
			{
				std::cout << "Client disconnected: " << _pollFds[i].fd << std::endl;
				_pollFds.erase(_pollFds.begin() + i);
				removeClient(client);
				continue;
			}
			buffer[bytes] = '\0';
			_clientList[client]->appendToBuffer(buffer, bytes);
			while (_clientList[client]->extractedLine(line))
			{
				// IMPLEMENTACIÓN CON PARSER
				ParsedCommand cmd = Parser::parse(line);
				
				if (!cmd.isValid) {
					std::cout << "Invalid message from client[" << _clientList[client]->getFd() << "]: " << line << std::endl;
					continue; // Ignorar mensajes inválidos
				}
				// Aquí se procesaría el comando válido

				std::cout << "Client[" << _clientList[client]->getFd() << "] -> " << cmd.command;
				if (!cmd.params.empty()) {
					std::cout << " (params: " << cmd.params.size() << ")";
				}
				std::cout << std::endl;
				if (!processCommand(_clientList[client], cmd)) {
					std::cout << "Client disconnected: " << _pollFds[i].fd << std::endl;
					_pollFds.erase(_pollFds.begin() + i);
					removeClient(client);
					break;
				}
				// IMPLEMENTACIÓN ANTERIOR 
				/*
			//	if (line == "CAP LS 302")
			//		sendMessage(_clientList[client]->getFd(), ":best.super.server.ever CAP * LS :");
			//	ejecutarComando()
				if (!wellcome(_clientList[client], line))
				{
					std::cout << "Client disconnected: " << _pollFds[i].fd << std::endl;
					_pollFds.erase(_pollFds.begin() + i);
					removeClient(client);
					break;
				}
				std::cout << "Client[" << _clientList[client]->getFd() <<"]: " << line << std::endl;
				*/
			}
		}
		_pollFds[i].revents = 0;
		i++;
	}
}

// processCommand para testing del parser
bool Server::processCommand(Client *client, const ParsedCommand &cmd)
{
	// Imprimir parámetros para debugging
	for (size_t i = 0; i < cmd.params.size(); ++i) {
		std::cout << "  Param[" << i << "]: '" << cmd.params[i] << "'" << std::endl;
	}
	
	// Manejar comandos básicos para irssi
	if (cmd.command == "CAP") {
		if (cmd.params.size() >= 2 && cmd.params[1] == "LS") {
			// Responder a CAP LS - no ofrecemos capabilities
			sendMessage(client->getFd(), ":server CAP * LS :");
		} else if (cmd.params.size() >= 2 && cmd.params[1] == "END") {
			// Cliente termina negociación de capabilities
			sendMessage(client->getFd(), ":server CAP * ACK :");
		}
	}
	else if (cmd.command == "PASS") {
		if (cmd.params.size() >= 2) {
			std::string password = cmd.params[1];
			if (password == _password) {
				client->Authenticate();
				std::cout << "Client " << client->getFd() << " authenticated" << std::endl;
			} else {
				sendMessage(client->getFd(), ":server 464 * :Password incorrect");
				return false; // Desconectar
			}
		}
	}
	else if (cmd.command == "NICK") {
		if (cmd.params.size() >= 2) {
			std::string nick = cmd.params[1];
			client->setField("NICK", nick);
			std::cout << "Client " << client->getFd() << " set nick: " << nick << std::endl;
		}
	}
	else if (cmd.command == "USER") {
		if (cmd.params.size() >= 5 && client->isAuthenticated()) {
			std::string username = cmd.params[1];
			std::string realname = cmd.params[4]; // :Real Name
			client->setField("USER", username);
			
			// Enviar welcome messages
			std::string nick = client->getField("NICK");
			sendMessage(client->getFd(), ":server 001 " + nick + " :Welcome to the IRC Network " + nick + "!");
			sendMessage(client->getFd(), ":server 002 " + nick + " :Your host is server, running version 1.0");
			sendMessage(client->getFd(), ":server 004 " + nick + " server 1.0");
			
			std::cout << "Client " << client->getFd() << " fully registered as " << nick << std::endl;
		} else if (!client->isAuthenticated()) {
			sendMessage(client->getFd(), ":server 464 * :Password incorrect");
		}
	}
	else if (cmd.command == "PING")
		sendMessage(client->getFd(), ":server PONG " + cmd.params[1]);
	else if (cmd.command == "QUIT") {
		std::cout << "Client " << client->getFd() << " quit" << std::endl;
		return false; // Desconectar
	}
	
	return true; // Mantener conexión
}

/*
ejecutarComando(cliente, std::string line)
{
	palabra;
	for (int i = 0; i < size; i++)
	{
		if (palabra == enum[i])
			if (!PASS && !client->isAuthenticated())
			{
				sendMessage(client->getFd(), ":best.super.server.ever 464 * :Password incorrect");
				return (0);
			}
			_commandos[i](cliente, line);
			break;
	}
}

ejecutarNick(cliente, std::string line)
{
	
}

*/
