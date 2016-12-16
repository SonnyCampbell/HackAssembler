#pragma once
#include <string>
#include <fstream>
#include <iostream>

enum CommandType
{
	ACommand,
	CCommand,
	LCommand
};



class Parser
{
	
public:
	Parser(std::string);;

	void Advance();
	bool HasMoreCommands();
	CommandType CommandType();

	std::string Symbol();
	std::string Dest();
	std::string Comp();
	std::string Jump();


	
	

private:
	std::ifstream myAssemblyFile;
	std::string line;


	std::string& ltrim(std::string&, const char* t );
	std::string& rtrim(std::string&, const char* t);
	std::string& trim(std::string&, const char* t);


};

