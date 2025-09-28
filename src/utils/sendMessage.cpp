#include "Ircserv.hpp"

int	sendMessage(int fd ,const std::string &msg)
{
	std::string	wire = msg + "\r\n";
	if (::send(fd, wire.c_str(), wire.size(), 0) == -1)
	{
		std::cerr << "Error: send failed" << std::endl;
		return (0);
	}
	return (1);
}