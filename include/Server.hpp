#ifndef SERVER_HPP
#define SERVER_HPP
#include "Ircserv.hpp"

class Server
{
	private:
		int	_fd;
		int	_port;
		std::string	_password;
		std::vector<Client>	_clientList;
		std::vector<Channel>	_channelList;
	public:
		Server(int port, const std::string &password);
		~Server();
		int	getFd();
		int	socketInit();
};

#endif
