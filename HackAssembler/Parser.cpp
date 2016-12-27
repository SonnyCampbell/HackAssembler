#include "Parser.h"

// trim from left
std::string& Parser::ltrim(std::string& s, const char* t)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from right
std::string& Parser::rtrim(std::string& s, const char* t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from left & right
std::string& Parser::trim(std::string& s, const char* t)
{
	return ltrim(rtrim(s, t), t);
}

const bool Parser::IsNumber(const std::string s) {
	return s.find_first_not_of("0123456789") == std::string::npos;
}