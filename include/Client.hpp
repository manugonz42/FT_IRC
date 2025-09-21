# ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Ircserv.hpp"

class Client
{
	private:
		int	fd;
		std::string	nick;
		std::string username;
		bool authenticated;

	public:
		Client();
		~Client();
};

#endif
