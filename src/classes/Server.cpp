#include "Ircserv.hpp"
// mejor que int, indibilisble, no se puede cortar la modificación
volatile sig_atomic_t g_exit;

Server::Server(int port, const std::string &password) : _fd(-1), _port(port), _password(password)
{
	_running = true;
	_serverName = ":server";
	fillCommandMap();
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

void Server::fillCommandMap()
{
	_commandMap["CAP"] = &Server::executeCap;
	_commandMap["JOIN"] = &Server::executeJoin;
	_commandMap["PASS"] = &Server::executePass;
	_commandMap["KICK"] = &Server::executeKick;
	_commandMap["MODE"] = &Server::executeMode;
	_commandMap["NICK"] = &Server::executeNick;
	_commandMap["NOTICE"] = &Server::executeNotice;
	_commandMap["PING"] = &Server::executePingPong;
	_commandMap["PRIVMSG"] = &Server::executePrivMsg;
	_commandMap["USER"] = &Server::executeUser;
	_commandMap["PART"] = &Server::executePart;
	_commandMap["VERSION"] = &Server::executeVersion;
	_commandMap["QUIT"] = &Server::executeQuit;
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

void	signalHandler(int signal)
{
	(void)signal;
	g_exit = 1;
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
	// Manejo de señales
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);
	std::signal(SIGQUIT, signalHandler);
	std::signal(SIGPIPE, SIG_IGN);
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
	g_exit = 0;
	while (!g_exit)
	{
		ready = poll(_pollFds.data(), _pollFds.size(), -1);
		if (ready < 0)
		{
			// añado && !g_exit
			// si se da el caso que entra aquí despues de recibir la señal saltaría al siguiente loop
			if (g_exit)
				break;
			std::cerr << "Error: poll failed" << std::endl;
			break;
		}
		acceptNewClient();
		processClientsInput();
	}
}

bool	Server::notifyToAllChannels(const std::string prefix, Client *client, const std::string msg)
{
	std::set<Client*> recipients;
	for (std::map<std::string, Channel *>::iterator it = _channelMap.begin();
		it != _channelMap.end(); ++it)
	{
		Channel *channel = it->second;
		if (channel->isClient(client->getField("NICK")))
		{
			std::map<std::string, Client *>::iterator itClients = _clientMap.begin();
			std::map<std::string, Client *>::iterator itClientsEnd = _clientMap.end();
			for (; itClients != itClientsEnd; ++itClients)
			{
				Client *toSend = itClients->second;
				if (channel->isClient(toSend->getField("NICK")) && toSend != client)
					recipients.insert(toSend);
			}
		}
	}
	for (std::set<Client *>::iterator it = recipients.begin();
		it != recipients.end(); ++it)
	{
		if (!::sendMessage(prefix, (*it)->getFd(), msg))
			return (false);
	}
	return (true);
}
bool	Server::removeClientFromChannels(Client *client)
{
	std::string clientNick = client->getField("NICK");
	for (std::map<std::string, Channel *>::iterator it = _channelMap.begin();
		it != _channelMap.end(); ++it)
	{
		Channel *channel = it->second;
		if (channel->isClient(client->getField("NICK")))
		{
			channel->removeClient(clientNick);
			channel->removeOperator(clientNick);
		}
	}
	return (true);
}