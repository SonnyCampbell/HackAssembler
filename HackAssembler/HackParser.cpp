#include "HackParser.h"


HackParser::HackParser(std::string filename)
{
	myAssemblyFile.open(filename);
}



void HackParser::Advance()
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


bool HackParser::HasMoreCommands()
{
	return false;
}

CommandType HackParser::TypeOfCommand()
{
	return ACommand;
}

std::string HackParser::Symbol()
{
	return "";
}
std::string HackParser::Dest()
{
	return "";
}
std::string HackParser::Comp()
{
	return "";
}
std::string HackParser::Jump()
{
	return "";

}




// trim from left
std::string& HackParser::ltrim(std::string& s, const char* t)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from right
std::string& HackParser::rtrim(std::string& s, const char* t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from left & right
std::string& HackParser::trim(std::string& s, const char* t)
{
	return ltrim(rtrim(s, t), t);
}

