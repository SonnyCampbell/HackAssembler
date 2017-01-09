#include "HackParser.h"
#include "VMParser.h"
#include "MachineCodeTranslate.h"
#include "Enums.h"
#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>

void NoFileError();
void WriteFile(Parser*, std::ofstream&);
Parser* SelectParser(std::string);
std::wstring string_to_wstring(const std::string&);


int main(int argc, char *argv[])
{
	/*if (argc == 1)

	{
		NoFileError();
	}*/
	
	// TODO: supply filename at runtime

	std::vector<std::wstring> files;
	
	std::string search_path = "../../../projects/08/FunctionCalls/StaticsTest/*.vm";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(string_to_wstring(search_path).c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				files.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	std::ofstream outputFile;
	 // TODO: change name of myAssemblyFile and machineCodeFile to more generic file
	outputFile.open("StaticsTest.asm", std::ios_base::app); // TODO: supply filename at runtime

	Parser *parser;	
	
	std::vector<std::wstring>::iterator it;
	it = std::find(files.begin(), files.end(), string_to_wstring("Sys.vm").c_str());

	if (it != files.end())
	{
		parser = new VMParser("../../../projects/08/FunctionCalls/StaticsTest/Sys.vm");
		files.erase(it);
		parser->WriteInit(outputFile);
		WriteFile(parser, outputFile);
	}

	for (auto file : files)
	{
		std::string f(file.begin(), file.end());
		parser = new VMParser("../../../projects/08/FunctionCalls/StaticsTest/" + f);
		WriteFile(parser, outputFile);
	}

	outputFile.close();

	/*if (filename.find(".vm") != filename.npos)
	{
		parser = new VMParser(filename);
	}
	else
	{
		parser = new HackParser(filename);
	}*/		 

	

	int n;
	std::cin >> n;
	return 0;
}


std::wstring string_to_wstring(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}

void WriteFile(Parser *parser, std::ofstream& outputFile)
{

	parser->Advance();
	while (parser->HasMoreCommands())
	{

		parser->WriteToFile(outputFile);
		parser->Clear();	
		parser->Advance(); 
	} 
	
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
