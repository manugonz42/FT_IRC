#include "../../include/Server.hpp"

int	Server::sendMessage(int fd ,const std::string &msg)
{
	std::string	wire = msg + "\r\n";
	if (::send(fd, wire.c_str(), wire.size(), 0) == -1)
	{
		std::cerr << "Error: send failed" << std::endl;
		return (0);
	}
	return (1);
}

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
			}
		}
		_pollFds[i].revents = 0;
		i++;
	}
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
