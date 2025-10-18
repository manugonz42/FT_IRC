#include "Ircserv.hpp"

void	Server::removeClient(size_t i)
{
	if (i >= _clientList.size())
		return;
	shutdown(_clientList[i]->getFd(), SHUT_WR);
	std::string clientNick = _clientList[i]->getField("NICK");
	std::map<std::string, Channel *>::iterator it = _channelMap.begin();
	for (;it != _channelMap.end(); it++)
		it->second->removeClient(clientNick);
	_clientMap.erase(strToUpper(clientNick));
	if (_clientList[i] != NULL)
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
		if (_pollFds.size() >= 1020)
		{
			::sendMessage(PREFIX, clientFd, "Server is full. Please wait...");
			std::cerr << "Client disconnected: " << clientFd << std::endl;
			close(clientFd);
			return ;
		}
		if (clientFd < 0)
			std::cerr << "Error: accept failed" << std::endl;
		else
		{
			Client *newClient = new Client(clientFd);
			if (!newClient)
			{
				close(clientFd);
				return;
			}
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
		bool clientRemoved = false;
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
				clientRemoved = true;
				continue;
			}
			else
			{
				_clientList[client]->appendToBuffer(buffer, bytes);
				while (_clientList[client]->extractedLine(line))
				{
					std::cout << "Client [" << _clientList[client]->getField("NICK") << "]";
					std::cout << "[" << _clientList[client]->getFd() << "] :" << line << std::endl; 
					ParsedCommand cmd = Parser::parse(line);
					
					if (!cmd.isValid)
					{
						if (!_clientList[client]->isAuthenticated())
						{
							sendNumeric(_clientList[client], 464, "");
							std::cout << "Client disconnected: " << _pollFds[i].fd << std::endl;
							_pollFds.erase(_pollFds.begin() + i);
							removeClient(client);
							clientRemoved = true;
							break;
						}
						std::cout << "Invalid message from client[" << _clientList[client]->getFd() << "]: " << line << std::endl;
						continue;
					}
					if (!executeCommand(_clientList[client], cmd))
					{
						std::cout << "Client disconnected: " << _pollFds[i].fd << std::endl;
						_pollFds.erase(_pollFds.begin() + i);
						removeClient(client);
						clientRemoved = true;
						break;
					}
				}
			}
		}
		if (clientRemoved)
			continue;    
		_pollFds[i].revents = 0;
		i++;
	}
}
