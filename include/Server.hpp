#ifndef SERVER_HPP
#define SERVER_HPP
#include "Ircserv.hpp"

class Server
{
	private:
		int	_fd;
		int	port;
		std::string	_password;
		std::vector<Client>	_clientList;
		std::vector<Channel>	_channelList;
	public:
		Server();
		~Server();
};

#endif
