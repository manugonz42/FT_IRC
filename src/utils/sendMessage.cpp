#include "Ircserv.hpp"

int	sendMessage(int fd ,const std::string &msg)
{
	/*int error = 0;
	socklen_t len = sizeof(error);
	int result = getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len);
	if (result != 0 || error != 0)
	{
		std::cout << "ERROR: Socket is not valid! getsockopt result: " << result << " error result: " << error << std::endl;
		return false;
	}
	else
		std::cout << "No hay error con el Socket" << std::endl;*/
	std::string	wire = PREFIX + msg + "\r\n";
	if (::send(fd, wire.c_str(), wire.size(), 0) == -1)
	{
		std::cerr << "Error: send failed" << std::endl;
		return (0);
	}
	return (1);
}