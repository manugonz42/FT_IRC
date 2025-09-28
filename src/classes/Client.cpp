#include "Ircserv.hpp"

Client::Client(int fd) : _fd(fd)
{
	_inputBuffer = "";
	_authenticated = false;
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

void	Client::setField(const std::string &type, const std::string &field)
{
	if (type == "NICK")
		_nick = field;
	if (type == "USER")
		_username = field;
}

std::string		Client::getField(const std::string &type)
{
	if (type == "NICK")
		return (_nick);
	else
		return (_username);
}

bool	Client::isAuthenticated()
{
	return (_authenticated);
}
void		Client::Authenticate()
{
	_authenticated = true;
}
