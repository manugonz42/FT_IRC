# ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Ircserv.hpp"

class Client
{
	private:
		int	_fd;
		std::string	_nick;
		std::string _username;
		bool _authenticated;

	public:
		Client(int fd);
		~Client();
		int getFd();
};

#endif
