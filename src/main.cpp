#include "../include/Ircserv.hpp"
// ONLY ACCEPT ONE SINGLE CLIENT
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}

	Server server(std::atoi(argv[1]), argv[2]);
	if (!server.socketInit())
		return (1);
	Client client(accept(server.getFd(), NULL, NULL));
	if (client.getFd() < 0)
	{
		std::cerr << "Error: Accept failed" << std::endl;
		return (1); 
	}
	std::cout << "Connected!" << std::endl;
	return (0);
}
