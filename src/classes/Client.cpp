#include "Ircserv.hpp"

Client::Client(int fd) : _fd(fd)
{
	_inputBuffer = "";
	_authenticated = false;
	_nick = "*";
	_username = "";
	_realname = "";
}

Client::~Client()
{
	if (_fd > -1)
		close (_fd);
}

int Client::getFd() const
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
	if (type == "REAL")
		_realname = field;
	if (type == "HOST")
		_hostname = field;	
}

std::string		Client::getField(const std::string &type) const
{
	if (type == "NICK")
		return (_nick);
	else if (type == "USER")
		return (_username);
	else if (type == "REAL")
		return (_realname);
	else
		return (_hostname);	
}

bool	Client::isAuthenticated() const
{
	return (_authenticated);
}
void		Client::Authenticate()
{
	_authenticated = true;
}
