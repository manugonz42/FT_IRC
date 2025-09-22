#include "../../include/Server.hpp"


Server::Server(int port, const std::string &password) : _fd(-1), _port(port), _password(password)
{
}

Server::~Server()
{
	if (_fd > -1)
		close(_fd);
}

int Server::getFd()
{
	return (_fd);
}

int	Server::socketInit()
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
	if (bind(_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::cerr << "Error: Bind failed" << std::endl;
		return (0);
	}
	if (listen(_fd, 10))
	{
		std::cerr << "Error: Listen failed" << std::endl;
		return (0);
	}
	std::cout << "Server listening on port " << _port << std::endl;
    return (1);
}
