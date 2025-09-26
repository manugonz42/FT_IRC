# ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Ircserv.hpp"

class Client
{
	private:
		int			_fd;
		std::string	_nick;
		std::string	_username;
		std::string	_inputBuffer;
		bool		_authenticated;


	public:
		Client(int fd);
		~Client();
		int			getFd();
		void		appendToBuffer(const char *, size_t);
		bool		extractedLine(std::string &line);
		std::string	getField(const std::string &type);
		bool		isAuthenticated();
		void		setField(const std::string &field, const std::string &nick);
		void		Authenticate();
};

#endif
