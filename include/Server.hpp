#ifndef SERVER_HPP
#define SERVER_HPP
#include "Ircserv.hpp"

class Server
{
	private:
		bool						_running;
		int							_fd;
		int							_port;
		std::string					_password;
		std::vector<struct pollfd>	_pollFds;
		std::vector<Client *>		_clientList;
		// std::vector<Channel *>		_channelList;
		void						acceptNewClient();
		void						removeClient(size_t i);
		void						processClientsInput();
		void						shutdownClients();
		int							sendMessage(int fd ,const std::string &msg);
		int							wellcome(Client *client ,const std::string &request);

	public:
		Server(int port, const std::string &password);
		~Server();
		int							getFd();
		int							socketInit();
		void						run();
};

#endif
