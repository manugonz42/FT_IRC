#ifndef DEFINES_HPP
# define DEFINES_HPP

# define PREFIX ":server "
# define SERVER_VERSION "1.0"
# define DEBUGLEVEL "0"
# define COMMENTS "ft_irc server - 42 School project"

# define VALID_IRC_COMMANDS \
	"CAP", "PASS", "NICK", "USER", "JOIN", "PART", "KICK", "INVITE", "TOPIC", \
	"PRIVMSG", "NOTICE", "QUIT", "PING", "VERSION", "MODE"

# define VALID_IRC_COMMANDS_COUNT 15

enum LoginStatus
{
	UNREGISTERED,
	PASS_SENT,
	NICK_SENT,
	USER_SENT,
	REGISTERED
};

typedef enum e_command
{
	CAP,
	PASS,
	NICK,
	USER,
	JOIN,
	PART,
	PRIVMSG,
	NOTICE,
	QUIT,
	PING,
	PONG,
	VERSION,
	TIME,
	MODE,
	KICK,
	INVITE,
	TOPICmake
}	t_command;

#endif