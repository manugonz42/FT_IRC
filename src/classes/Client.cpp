#include "../../include/Server.hpp"

Client::Client(int fd) : _fd(fd)
{
	_inputBuffer = "";
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

void	Client::appendToBuffer(const char *chunk, size_t bytes)
{
	_inputBuffer.append(chunk, bytes);
}

void	Client::printBuffer()
{
	std::cout << _inputBuffer << std::endl;
}

bool	Client::extractedLine(std::string &line)
{
	size_t	pos;
	
	pos = _inputBuffer.find("\r\n");
	if (pos == std::string::npos)
		return (false);
	line = _inputBuffer.substr(0, pos);
	_inputBuffer.erase(0, pos + 2);
	return (true);
}