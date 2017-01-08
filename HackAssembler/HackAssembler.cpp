#include "HackParser.h"
#include "VMParser.h"
#include "MachineCodeTranslate.h"
#include "Enums.h"
#include <iostream>
#include <fstream>

void NoFileError();
void WriteFile(Parser*);
Parser* SelectParser(std::string);


int main(int argc, char *argv[])
{
	/*if (argc == 1)
	{
		NoFileError();
	}*/
	
	// TODO: supply filename at runtime
	string filename = "SimpleFunction.vm";
	Parser *parser;

	if (filename.find(".vm") != filename.npos)
	{
		parser = new VMParser(filename);
	}
	else
	{
		parser = new HackParser(filename);
	}		 

	WriteFile(parser);

	int n;
	std::cin >> n;
	return 0;
}




void WriteFile(Parser *parser)
{
	//HackParser parser("Pong.asm");

	std::ofstream outputFile; // TODO: change name of myAssemblyFile and machineCodeFile to more generic file
	outputFile.open("SimpleFunction.asm"); // TODO: supply filename at runtime

	parser->Advance();
	while (parser->HasMoreCommands())
	{

		parser->WriteToFile(outputFile);
		parser->Clear();	
		parser->Advance(); 
	} 
	outputFile.close();
	std::cout << "File written successfully." << std::endl;
}

Parser* SelectParser(std::string filename)
{
	
	if (filename.find(".vm") != filename.npos) 
	{ 
		VMParser vmParser(filename);
		return &vmParser;
	}
	else
	{
		HackParser hackParser(filename);
		return &hackParser;
	}
}


void NoFileError()
{
	std::cout << "No file given specified to compile. Please restart and specify an assembly file." << std::endl;
}
