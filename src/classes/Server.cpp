#include "../../include/Server.hpp"


Server::Server(int port, const std::string &password) : _fd(-1), _port(port), _password(password)
{
	_running = true;
}

Server::~Server()
{
	shutdownClients();
	if (_fd > -1)
	{
		close(_fd);
		_fd = -1;
	}
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

//	Prepare the socket and put it into listening mode
//
//
//	socket():
//		Creates a socket for network communication
//			AF_INET: For IPv4 address (DOMAIN)
//			SOCK_STREAM: For TCP connection (TYPE)
//			0: Tells the kernel that it will use the default protocol for this domain/type
//	bind():
//		Associates your socket with a specific local address (IP + port)
//		After bind(), the OS knows which interface/port your server will use
//		so it can accept incoming connections on that address
//			sockaddr_in: a structure that contains the info that bind() needs:
//				sin_family: Address family 
//				AF_INET: IPv4
//				sin_addr.s_addr: is the 32-bit IPv4 address stored in network byte order
//					INADDR_ANY: Means "accept all local interfaces" (0.0.0.0)
//	listen():
//		Puts the socket into listening mode for incoming connections
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

// Function that accepts a new client
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
	size_t	i = 1;

	while (i < _pollFds.size())
	{
		size_t	client = i - 1;
		close(_pollFds[i].fd);
		_pollFds.erase(_pollFds.begin() + i);
		removeClient(client);
	}
}

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

//	This is only provisional!
//	Must give a name to our server!
int		Server::wellcome(Client *client, const std::string &request)
{
	size_t i = 0;
	if (request == "CAP LS 302")
	{
		std::cout << "sending handshake..." << std::endl;
		sendMessage(client->getFd(), ":best.super.server.ever CAP * LS :");
	}
	else if (request == "JOIN :")
	{
		std::cout << "sending join error..." << std::endl;
		sendMessage(client->getFd(), ":best.super.server.ever 461 * JOIN :Not enough parameters");
	}
	else if ((i = request.find("NICK")) != std::string::npos)
	{
		client->setField("NICK", request.substr(i + 5, request.length()));
	}
	else if ((i = request.find("USER")) != std::string::npos)
	{
		client->setField("USER", "\"username\"");
		std::cout << "sending welcome..." << std::endl;
		sendMessage(client->getFd(), ":best.super.server.ever 001 " + client->getField("NICK") + " :Welcome to the Internet Relay Network!");
	}
	return (1);
}
//	Las “capabilities” (o “caps”) son un mecanismo de IRCv3 que permite negociar
//	funciones opcionales entre cliente y servidor antes de completar el registro.
//	El cliente envía CAP para preguntar qué extensiones soporta el servidor,
//	activar o desactivar algunas y, en base a eso, ambos lados saben qué
//	comandos o tags adicionales pueden usar
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
				wellcome(_clientList[client], line);
				std::cout << "Client[" << _clientList[client]->getFd() <<"]: " << line << std::endl;
			}
		}
		_pollFds[i].revents = 0;
		i++;
	}
	
}

//	Server main loop. Waits for sockets to become ready, accept new clients,
//	reads data from established clients and saves each complete line into the client buffer.
//	pollfd struct:
//		Contains the data that poll() needs.
//		It has 3 fields:
//			1) fd: The file descriptor that will be monitored.
//			2) events:
//				Bitmask that specifies which events to monitor on the descriptor:
//					POLLIN: Notifies when data can be read without blocking (e.g., accept ready).
//					POLLOUT: Notifies when data can be written without blocking.
//			3) revents: Filled by poll(). This shows the events that actually occurred.
//	poll():
//		Waits for activity on a set of fds.
//	**push_back:
//		Adds a new item to the back of the vector.
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

