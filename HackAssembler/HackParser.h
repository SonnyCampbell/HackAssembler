#pragma once
#include "Parser.h"
#include "Enums.h"
#include "ExceptionHandler.h"
#include "Constants.h"
#include <string>
#include <fstream>
#include <iostream>

using std::string;

class HackParser :
	public Parser
{
public:
	HackParser(string);
	//~HackParser();

	void Advance();
	bool HasMoreCommands();

	CommandType TypeOfCommand();

	string Symbol();
	string Dest();
	string Comp();
	string Jump();
	void Clear();


protected:
	CommandType myCommandType;
	std::ifstream myAssemblyFile;
	

	string& ltrim(string&, const char* t);
	string& rtrim(string&, const char* t);
	string& trim(string&, const char* t);


private:
	string currentLine;
	string myAInstruction;
	string myJumpInstruction;
	string myDestInstruction;
	string myCompInstruction;
	string myLoopInstruction;

	// TODO: Set to true for full enviroment
	bool firstPass = false;

	void GetNextLine();
	void GetNextCommand();
	void ParseLine();
	void ParseLabels();
	void ParseCInstruction();


};

