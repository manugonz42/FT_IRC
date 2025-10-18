#ifndef SERVER_HPP
#define SERVER_HPP
#include "Ircserv.hpp"
#include "Channel.hpp"

struct ParsedCommand;
class Channel;

class Server
{
	private:
		bool						_running;
		std::string			_serverName;
		int							_fd;
		const int					_port;
		const std::string			_password;
		std::vector<struct pollfd>	_pollFds;
		std::vector<Client *>		_clientList;
		std::map<std::string, Client *>	_clientMap;
		std::map<std::string, Channel *> _channelMap;
		std::map<std::string, bool (Server::*)(Client *client, const ParsedCommand &cmd)> _commandMap;

		void						acceptNewClient();
		void						removeClient(size_t i);
		void						processClientsInput();
		void						shutdownClients();
		void						fillCommandMap();
		bool						executeCommand(Client *client, const ParsedCommand &cmd);
		bool						createChannel(const Client& client, const std::string& name);

		bool						executePingPong(Client *client, const ParsedCommand &cmd);
		bool						executePass(Client *client, const ParsedCommand &cmd);
		bool						executeCap(Client *client, const ParsedCommand &cmd);
		bool						executeNick(Client *client, const ParsedCommand &cmd);
		bool						executeUser(Client *client, const ParsedCommand &cmd);
		bool						executeJoin(Client *client, const ParsedCommand &cmd);
		bool						executePart(Client *client, const ParsedCommand &cmd);
		bool						executeKick(Client *client, const ParsedCommand &cmd);
		bool						executeInvite(Client *client, const ParsedCommand &cmd);
		bool						executeTopic(Client *client, const ParsedCommand &cmd);
		bool						executePrivMsg(Client *client, const ParsedCommand &cmd);
		bool						executeNotice(Client *client, const ParsedCommand &cmd);
		bool						executeQuit(Client *client, const ParsedCommand &cmd);
		bool						executeVersion(Client *client, const ParsedCommand &cmd);
		bool						executeMode(Client *client, const ParsedCommand &cmd);
		bool						sendWelcome(Client *client);
		bool						notifyToAllChannels(const std::string prefix, Client *client, const std::string msg);
		bool						removeClientFromChannels(Client *client);					
	public:
		Server(int port, const std::string &password);
		~Server();
		int							getFd() const;
		int							socketInit();
		void						run();
};

#endif

