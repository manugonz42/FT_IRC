#include "Ircserv.hpp"

Parser::Parser() {}
Parser::Parser(const Parser& other) { (void)other; }
Parser& Parser::operator=(const Parser& other) { (void)other; return *this; }
Parser::~Parser() {}

ParsedCommand Parser::parse(const std::string& rawMessage)
{
	ParsedCommand result;
	
	std::string trimmed = trim(rawMessage);

	if (!hasValidFormat(trimmed))
	{
		result.isValid = false;
		return result;
	}
	
	std::string cleaned = removePrefix(trimmed, result);
	if (cleaned.empty())
	{
		result.isValid = false;
		return result;
	}
	
	std::string command = extractCommand(cleaned);
	if (command.empty())
	{
		result.isValid = false;
		return result;
	}
	
	if (!isValidCommand(command))
	{
		result.isValid = false;
		return result;
	}
	
	std::vector<std::string> tokens = tokenize(cleaned);
	if (tokens.size() > 15) 
	{
		result.isValid = false;
		return result;
	}
	result.command = command;
	result.params = tokens;

	result.isValid = true;
	
	return result;
}

bool Parser::isValidCommand(const std::string& command)
{
	static const char* VALID_COMMANDS[] = { VALID_IRC_COMMANDS };
	
	for (size_t i = 0; i < VALID_IRC_COMMANDS_COUNT; ++i)
	{
		if (command == VALID_COMMANDS[i])
			return true;
	}
	return false;
}
std::string Parser::extractCommand(const std::string& message)
{
	size_t end = message.find_first_of(" ");
	if (end == std::string::npos)
		end = message.length(); 

	std::string command = message.substr(0, end);
	command = strToUpper(command);

	return command;
}

std::vector<std::string> Parser::tokenize(const std::string& message)
{
	std::vector<std::string> tokens;
	size_t pos = 0;
	
	while (pos < message.length())
	{
		pos = message.find_first_not_of(" ", pos);
		
		if (pos == std::string::npos)
			break;
		
		if (message[pos] == ':')
		{
			tokens.push_back(message.substr(pos));
			break;
		}
		
		size_t start = pos;
		pos = message.find_first_of(" ", start);

		tokens.push_back(message.substr(start, pos - start));
	}
	
	return tokens;
}

std::string Parser::trim(const std::string& str)
{
	size_t start = str.find_first_not_of(" ");
	size_t end = str.find_last_not_of(" ");
	
	if (start == std::string::npos || end == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}

std::string Parser::removePrefix(const std::string& message, ParsedCommand& result)
{
	if (message[0] != ':')
		return message;
	
	size_t spacePos = message.find_first_of(" ");
	if (spacePos == std::string::npos)
		return ""; 

	result.prefix = message.substr(1, spacePos - 1);
	
	size_t start = message.find_first_not_of(" ", spacePos);
	
	return message.substr(start);
}

bool Parser::hasValidFormat(const std::string& message)
{
	if (message.find('\0') != std::string::npos)
		return false;
	
	if (message.empty() || message.length() > 512)
		return false;

	return true;
}