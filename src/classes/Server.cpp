#include "../../include/Server.hpp"


Server::Server(int port, const std::string &password) : _fd(-1), _port(port), _password(password)
{
}

Server::~Server()
{
	if (_fd > -1)
		close(_fd);
}
