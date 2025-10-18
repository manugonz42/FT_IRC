# ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Ircserv.hpp"

class Client
{
	private:
		int			_fd;
		std::string	_nick;
		std::string	_username;
		std::string	_realname;
		std::string	_inputBuffer;
		std::string	_hostname;
		std::string	_prefix;
		LoginStatus	_loginStatus;
		bool		_authenticated;


	public:
		Client(int fd);
		~Client();
		int			getFd() const;
		void		appendToBuffer(const char *, size_t);
		bool		extractedLine(std::string &line);
		std::string	getField(const std::string &type) const;
		bool		isAuthenticated() const;
		void		setField(const std::string &field, const std::string &nick);
		void		Authenticate();
		// Getters y setters
		void		setLoginStatus(LoginStatus status);
		LoginStatus	getLoginStatus() const;
		void		setPrefix(void);
};

#endif
