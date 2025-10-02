#ifndef DEFINES_HPP
# define DEFINES_HPP

# define PREFIX ":server "

# define VALID_IRC_COMMANDS \
    "CAP", "PASS", "NICK", "USER", "JOIN", "PART", "KICK", "INVITE", "TOPIC", \
    "PRIVMSG", "NOTICE", "QUIT", "PING", "VERSION", "TIME", "MODE"

# define VALID_IRC_COMMANDS_COUNT 16

enum LoginStatus
{
    UNREGISTERED,   // Recién conectado
    PASS_SENT,      // Contraseña enviada
    NICK_SENT,      // Nickname enviado  
    USER_SENT,      // Usuario enviado (NUEVO)
    REGISTERED      // Login completo
};
# define VALID_IRC_COMMANDS_COUNT 16

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
}	t_command;

#endif