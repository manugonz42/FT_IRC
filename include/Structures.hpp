#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>
#include <vector>

struct ParsedCommand
{
	std::string					command;
	std::vector<std::string>	params;
	std::string					prefix;
	bool						isValid;

	ParsedCommand() : isValid(false) {}
};

#endif
