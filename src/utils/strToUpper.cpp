#include "Ircserv.hpp"

std::string strToUpper(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::toupper(result[i]);
    }
    return result;
}
