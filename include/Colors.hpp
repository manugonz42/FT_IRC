#ifndef COLORS_HPP
# define COLORS_HPP

// ========== TEXT COLORS ==========
// Basic
# define BLACK		"\033[30m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW		"\033[33m"
# define BLUE		"\033[34m"
# define MAGENTA	"\033[35m"
# define CYAN		"\033[36m"
# define WHITE		"\033[37m"

// Bold
# define BRIGHT_BLACK	"\033[90m"
# define BRIGHT_RED		"\033[91m"
# define BRIGHT_GREEN	"\033[92m"
# define BRIGHT_YELLOW	"\033[93m"
# define BRIGHT_BLUE	"\033[94m"
# define BRIGHT_MAGENTA	"\033[95m"
# define BRIGHT_CYAN	"\033[96m"
# define BRIGHT_WHITE	"\033[97m"

// ========== BACKGROUND COLORS ==========
# define BG_BLACK	"\033[40m"
# define BG_RED		"\033[41m"
# define BG_GREEN	"\033[42m"
# define BG_YELLOW	"\033[43m"
# define BG_BLUE	"\033[44m"
# define BG_MAGENTA	"\033[45m"
# define BG_CYAN	"\033[46m"
# define BG_WHITE	"\033[47m"

// ========== TEXT FORMAT ==========
# define RESET		"\033[0m"
# define BOLD		"\033[1m"
# define DIM		"\033[2m"
# define ITALIC		"\033[3m"
# define UNDERLINE	"\033[4m"
# define BLINK		"\033[5m"
# define REVERSE	"\033[7m"
# define STRIKETHROUGH	"\033[9m"

// ========== CONVENIENCE MACROS ==========
// Log messages
# define LOG_INFO(msg)		std::cout << CYAN << "[INFO] " << RESET << msg << std::endl
# define LOG_ERROR(msg)		std::cerr << RED << "[ERROR] " << RESET << msg << std::endl
# define LOG_WARNING(msg)	std::cout << YELLOW << "[WARNING] " << RESET << msg << std::endl
# define LOG_SUCCESS(msg)	std::cout << GREEN << "[SUCCESS] " << RESET << msg << std::endl
# define LOG_DEBUG(msg)		std::cout << MAGENTA << "[DEBUG] " << RESET << msg << std::endl

// Highlight
# define HIGHLIGHT(text)	BOLD << YELLOW << text << RESET
# define ERROR_TEXT(text)	BOLD << RED << text << RESET
# define SUCCESS_TEXT(text)	BOLD << GREEN << text << RESET
# define INFO_TEXT(text)	BOLD << CYAN << text << RESET

// Specific commands
# define IRC_COMMAND(cmd)	BOLD << BLUE << cmd << RESET
# define IRC_NICK(nick)		BOLD << MAGENTA << nick << RESET
# define IRC_CHANNEL(chan)	BOLD << GREEN << chan << RESET
# define IRC_MESSAGE(msg)	WHITE << msg << RESET

// ========== MACROS ==========
# define HEADER_STYLE	BOLD << UNDERLINE << CYAN
# define CLIENT_STYLE	BOLD << BLUE
# define SERVER_STYLE	BOLD << GREEN
# define ERROR_STYLE	BOLD << RED
# define WARNING_STYLE	BOLD << YELLOW

// ========== UTILS ==========
# define CLEAR_LINE		"\033[2K\r"
# define MOVE_UP		"\033[1A"
# define MOVE_DOWN		"\033[1B"
# define SAVE_CURSOR	"\033[s"
# define RESTORE_CURSOR	"\033[u"

#endif