#pragma once
#include "Parser.h"
#include "Constants.h"
#include "Enums.h"
#include "ExceptionHandler.h"
#include <iostream>

using std::string;

class VMParser :
	public Parser
{
public:
	VMParser(string);
	//~VMParser();

	void Advance();
	bool HasMoreCommands();
	void WriteToFile(std::ofstream&);
	void Clear();
	CommandType TypeOfCommand();

private:
	CommandType myCommandType;
	std::ifstream myAssemblyFile;
	std::string currentLine;

	void GetNextLine();
	void GetNextCommand();
	void ParseLine();
	void SetRegisterArgument(std::string);

	std::string registerArg;
	std::string label;

	std::string arg2;
	std::string inputFileName;

	int jumpCount = 0;

};

