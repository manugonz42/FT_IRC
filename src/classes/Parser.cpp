#include "../include/Parser.hpp"

// Constructor privado - no implementado para prevenir instanciación
Parser::Parser() {}
Parser::Parser(const Parser& other) { (void)other; }
Parser& Parser::operator=(const Parser& other) { (void)other; return *this; }
Parser::~Parser() {}

// Función principal de parsing
// Parsea un mensaje IRC completo y devuelve una estructura ParsedComand
ParsedCommand Parser::parse(const std::string& rawMessage)
{
	ParsedCommand result;
	
	std::string trimmed = trim(rawMessage);

	// Validación básica de formato
	if (!hasValidFormat(trimmed)) {
		result.isValid = false;
		return result;
	}
	
	// Eliminar prefix si existe 
	std::string cleaned = removePrefix(trimmed);
	if (cleaned.empty()) {
		result.isValid = false;
		return result;
	}
	
	// Extraer comando 
	std::string command = extractCommand(cleaned);
	if (command.empty()) {
		result.isValid = false;
		return result;
	}
	
	// Validar que el comando sea válido
	if (!isValidCommand(command)) {
		result.isValid = false;
		return result;
	}
	
	// Tokenizar el mensaje completo (pasamos cleaned)
	std::vector<std::string> tokens = tokenize(cleaned);
	
	// Llenar la estructura resultado
	result.command = command;
	result.params = tokens;

	result.isValid = true;
	
	return result;
}

// Valida si un comando es válido
bool Parser::isValidCommand(const std::string& command)
{
	// Comparar contra la lista de comandos válidos
	static const char* VALID_COMMANDS[] = { VALID_IRC_COMMANDS };
	
	for (size_t i = 0; i < VALID_IRC_COMMANDS_COUNT; ++i) {
		if (command == VALID_COMMANDS[i])
			return true;
	}
	return false;
}

// Para comandos, intentar unificar enviando requisitos
bool Parser::isValidMessage(const std::string& message) //
{
	(void)message;  // Evitar warning unused parameter
	// TODO: Implementar validación del formato completo del mensaje
	// - Verificar longitud máxima (512 caracteres según RFC)
	// - Verificar caracteres válidos
	// - Verificar estructura general
	
	return false;
}

// Extrae solo el comando de un mensaje IRC
std::string Parser::extractCommand(const std::string& message)
{
	// Encontrar el final de la primera palabra (el comando)
	size_t end = message.find_first_of(" \t\r\n");
	if (end == std::string::npos)
		end = message.length(); // Comando hasta el final

	// Extraer y convertir a mayúsculas
	std::string command = message.substr(0, end);
	for (size_t i = 0; i < command.length(); ++i)
		command[i] = std::toupper(command[i]);

	return command;
}

// Divide el mensaje en tokens separados por espacios
std::vector<std::string> Parser::tokenize(const std::string& message)
{
	// TODO: Implementar tokenización
	// - Dividir por espacios
	// - Manejar parámetros que empiecen con ':' (trailing parameter, siempre el ultimo)
	// - Mantener espacios dentro del trailing parameter
	std::vector<std::string> tokens;
	size_t pos = 0;
	
	while (pos < message.length()) {
		// Saltar whitespace al principio
		pos = message.find_first_not_of(" \t\r\n", pos);
		
		// Si llegamos al final, salir
		if (pos == std::string::npos)
			break;
		
		// Si encontramos ':', es el trailing parameter (resto del mensaje)
		if (message[pos] == ':') {
			tokens.push_back(message.substr(pos));
			break;
		}
		
		// Buscar el final de este token (siguiente whitespace)
		size_t start = pos;
		pos = message.find_first_of(" \t\r\n", start);

		// Agregar el token encontrado
		tokens.push_back(message.substr(start, pos - start));
	}
	
	return tokens;
}

// Elimina espacios en blanco al principio y final de una cadena
std::string Parser::trim(const std::string& str)
{
	// TODO: Implementar función trim
	// - Eliminar espacios, tabs, \r, \n del principio y final
	const std::string whitespace = " \t\r\n";
	size_t start = str.find_first_not_of(whitespace);
	size_t end = str.find_last_not_of(whitespace);
	
	if (start == std::string::npos || end == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}

// Elimina el prefix IRC si existe (formato :prefix COMMAND params)
std::string Parser::removePrefix(const std::string& message)
{
	// Si no empieza con ':', no hay prefix
	if (message[0] != ':')
		return message;
	
	// Buscar el primer whitespace después del prefix
	size_t spacePos = message.find_first_of(" \t\r\n");
	if (spacePos == std::string::npos)
		return ""; // Solo hay prefix, no hay comando
	
	// Saltar todos los whitespace después del prefix
	size_t start = message.find_first_not_of(" \t\r\n", spacePos);
	
	// Devolver mensaje sin prefix
	return message.substr(start);
}

// Verifica si el mensaje tiene un formato válido básico
bool Parser::hasValidFormat(const std::string& message)
{
	// Asume que el mensaje ya está trimmed desde parse()
	if (message.find('\0') != std::string::npos)
		return false;
	
	// Ya no necesitamos trim aquí, se hace en parse()
	if (message.empty() || message.length() > 512)
		return false;

	return true;
}