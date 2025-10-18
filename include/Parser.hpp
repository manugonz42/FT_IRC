#ifndef PARSER_HPP
#define PARSER_HPP

#include "Ircserv.hpp"

class Parser
{
	private:
		Parser();
		Parser(const Parser& other);
		Parser& operator=(const Parser& other);
		~Parser();

	public:
		static ParsedCommand		parse(const std::string& rawMessage);
		
		static bool					isValidCommand(const std::string& command);
		static bool					isValidMessage(const std::string& message);
		
		static std::string			extractCommand(const std::string& message);
		
		static std::vector<std::string>	tokenize(const std::string& message);
		static std::string			trim(const std::string& str);
		static std::string			removePrefix(const std::string& message, ParsedCommand& result);
		static bool					hasValidFormat(const std::string& message);
};

#endif