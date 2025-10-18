#include "Ircserv.hpp"

bool isValidLength(const std::string &str, size_t minLen, size_t maxLen)
{
    return str.length() >= minLen && str.length() <= maxLen;
}