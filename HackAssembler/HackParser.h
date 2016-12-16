#pragma once
#include "Parser.h"
#include "Enums.h"
#include <string>
#include <fstream>
#include <iostream>

class HackParser :
	public Parser
{
public:
	HackParser(std::string);
	//~HackParser();

	void Advance();
	bool HasMoreCommands();

	CommandType TypeOfCommand();

	std::string Symbol();
	std::string Dest();
	std::string Comp();
	std::string Jump();


protected:
	std::ifstream myAssemblyFile;
	std::string line;

	std::string& ltrim(std::string&, const char* t);
	std::string& rtrim(std::string&, const char* t);
	std::string& trim(std::string&, const char* t);

};

