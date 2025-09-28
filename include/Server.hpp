#ifndef SERVER_HPP
#define SERVER_HPP
#include "Ircserv.hpp"

// Forward declaration para evitar dependencia circular
struct ParsedCommand;

class Server
{
	private:
		bool						_running;
		std::string					_serverName;
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
		bool						processCommand(Client *client, const ParsedCommand &cmd);
		int							wellcome(Client *client ,const std::string &request);  // Método original mantenido

	public:
		Server(int port, const std::string &password);
		~Server();
		int							getFd();
		int							socketInit();
		void						run();
};

#endif
