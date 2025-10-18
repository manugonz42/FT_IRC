#include "Ircserv.hpp"

int	sendMessage(std::string prefix, int fd ,const std::string &msg)
{
	std::string	str = prefix + msg + "\r\n";
	std::cout << "Sending : " << "\"" + prefix + msg + "\"" << std::endl;
	if (::send(fd, str.c_str(), str.size(), 0) == -1)
	{
		std::cerr << "Error: send failed" << std::endl;
		return (0);
	}
	return (1);
}