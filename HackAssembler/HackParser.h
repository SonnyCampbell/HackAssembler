#pragma once
#include "Parser.h"
#include "Enums.h"
#include "ExceptionHandler.h"
#include "MachineCodeTranslate.h"
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

	void InitialiseSymbolTable();


protected:
	CommandType myCommandType;
	std::ifstream myAssemblyFile;
	

	string& ltrim(string&, const char* t);
	string& rtrim(string&, const char* t);
	string& trim(string&, const char* t);
	const bool IsNumber(const string s);


private:
	string currentLine;
	string myAInstruction;
	string myJumpInstruction;
	string myDestInstruction;
	string myCompInstruction;
	string myLoopInstruction;

	std::unordered_map<string, string> SymbolTable;
	

	// TODO: Set to true for full enviroment
	bool firstPass;
	int lineNumber;
	int RAMSymbolNumber;

	void GetNextLine();
	void GetNextCommand();
	void ParseLine();
	void ParseLabels();
	void ParseCInstruction();


};

