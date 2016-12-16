#pragma once
#include "Enums.h"
#include <string>
#include <fstream>

class Parser
{

public:
	virtual void Advance() = 0;
	virtual bool HasMoreCommands() = 0;
	virtual CommandType TypeOfCommand() = 0;

	virtual std::string Symbol() = 0;
	virtual std::string Dest() = 0;
	virtual std::string Comp() = 0;
	virtual std::string Jump() = 0;


protected:
	std::ifstream myAssemblyFile;
	std::string currentLine;

	virtual std::string& ltrim(std::string&, const char* t ) = 0;
	virtual std::string& rtrim(std::string&, const char* t) = 0;
	virtual std::string& trim(std::string&, const char* t) = 0;


};

