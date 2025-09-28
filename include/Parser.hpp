#ifndef PARSER_HPP
#define PARSER_HPP

#include "Ircserv.hpp"

class Parser
{
	private:
		// Constructor privado para prevenir instanciación
		Parser();
		Parser(const Parser& other);
		Parser& operator=(const Parser& other);
		~Parser();

	public:
		// Función principal de parsing
		static ParsedCommand		parse(const std::string& rawMessage);
		
		// Funciones de validación
		static bool					isValidCommand(const std::string& command);
		static bool					isValidMessage(const std::string& message);
		
		// Función de extracción de comando
		static std::string			extractCommand(const std::string& message);
		
		// Funciones auxiliares
		static std::vector<std::string>	tokenize(const std::string& message);
		static std::string			trim(const std::string& str);
		static std::string			removePrefix(const std::string& message);
		static bool					hasValidFormat(const std::string& message);
};

#endif