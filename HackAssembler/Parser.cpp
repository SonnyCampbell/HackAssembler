#include "Parser.h"



Parser::Parser(std::string filename)
{
	myAssemblyFile.open(filename);
}


void Parser::Advance()
{
	if (!myAssemblyFile.eof())
	{
		std::getline(myAssemblyFile, line);

		while (line.find_first_not_of(' ') == std::string::npos && !myAssemblyFile.eof())
		{
			std::getline(myAssemblyFile, line);
		}

		line = trim(line, " \t\n\r\f\v");

		while (line[0] == '/' && line[1] == '/')
		{

			std::getline(myAssemblyFile, line);
		}

		line = trim(line, " \t\n\r\f\v");

		
	}
	else
	{
		//Handle EOF
		std::cout << "Reached end of file." << std::endl;
		return;
	}
}


bool Parser::HasMoreCommands()
{
	return false;
}

CommandType Parser::CommandType()
{
	return ACommand;
}

std::string Parser::Symbol()
{
	return "";
}
std::string Parser::Dest()
{
	return "";
}
std::string Parser::Comp()
{
	return "";
}
std::string Parser::Jump()
{
	return "";

}




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
