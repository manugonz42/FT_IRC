#include "../../include/Server.hpp"

Client::Client(int fd) : _fd(fd)
{
}

Client::~Client()
{
	if (_fd > -1)
		close (_fd);
}

int Client::getFd()
{
	return (_fd);
}