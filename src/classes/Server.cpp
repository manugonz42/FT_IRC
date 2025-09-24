#include "../../include/Server.hpp"


Server::Server(int port, const std::string &password) : _fd(-1), _port(port), _password(password)
{
	_running = true;
}

Server::~Server()
{
	size_t i = 0;
    while (i < _clientList.size()) {
        delete _clientList[i];
        ++i;
    }
    _clientList.clear();
	if (_fd > -1)
		close(_fd);
}

int		Server::getFd()
{
	return (_fd);
}

int		reusePort(int fd)
{
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
   		std::cerr << "setsockopt failed" << std::endl;
		return (-1);
	}
	return (0);
}

int		Server::socketInit()
{
	struct sockaddr_in addr;

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
	{
		std::cerr << "Error: Socket failed" << std::endl;
		return (0);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (reusePort(_fd) < 0)
		return (0);
	if (bind(_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::cerr << "Error: Bind failed" << std::endl;
		return (0);
	}
	if (listen(_fd, 10) < 0)
	{
		std::cerr << "Error: Listen failed" << std::endl;
		return (0);
	}
	std::cout << "Server listening on port " << _port << std::endl;
    return (1);
}

void Server::acceptNewClient()
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
			fcntl(clientFd, F_SETFL, O_NONBLOCK);
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

void Server::removeClient(size_t i)
{
	if (i >= _clientList.size())
		return;
	delete _clientList[i];
	_clientList.erase(_clientList.begin() + i);
}

void Server::shutdownClients()
{
	size_t i = 1;

	while (i < _pollFds.size())
	{
		close(_pollFds[i].fd);
		_pollFds.erase(_pollFds.begin() + i);
		removeClient(i - 1);
	}
}

void	Server::handleMsg()
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
			_clientList[client]->appendToBuffer(buffer, bytes);
			while (_clientList[client]->extractedLine(line))
			{
				std::cout << "Client[" << _clientList[client]->getFd() <<"]: " << line << std::endl;
			}
		}
		_pollFds[i].revents = 0;
		i++;
	}
	
}

//	- The pollfd struct contains the data that poll() needs.
//	It has 3 fields:
//		1) fd: The file descriptor that will be monitored.
//		2) events:
//			Bitmask that specifies which events to monitor on the descriptor:
//				POLLIN: Notifies when data can be read without blocking (e.g., accept ready).
//				POLLOUT: Notifies when data can be written without blocking.
//		3) revents: Filled by poll(). This shows the events that actually occurred.
//	- The push_back function adds a new item to the back of the vector.

void	Server::run()
{
	struct pollfd	serverPoll;
	int				ready;
	
	serverPoll.fd = _fd;
	serverPoll.events = POLLIN;
	serverPoll.revents = 0;
	_pollFds.push_back(serverPoll);
	while (_running)
	{
		ready = poll(_pollFds.data(), _pollFds.size(), -1);
		if (ready < 0)
		{
			if (errno == EINTR)
				continue;
			std::cerr << "Error: poll failed" << std::endl;
			break;
		}
		acceptNewClient();
		handleMsg();
	}
}

