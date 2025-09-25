#include "../include/Ircserv.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}
	
	Server	server(std::atoi(argv[1]), argv[2]);
	if (!server.socketInit())
		return (1);
	server.run();
	return (0);
}
