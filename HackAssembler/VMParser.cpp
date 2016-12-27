#include "VMParser.h"

VMParser::VMParser(string filename)
{

	myAssemblyFile.open(filename);

	size_t fileExt = filename.find_first_of(".");
	inputFileName = filename.substr(0, fileExt);
}


//VMParser::~VMParser()
//{
//}



void VMParser::Advance()
{
	if (HasMoreCommands())
	{
		GetNextCommand();
	}
	else
	{
		//Handle EOF
		std::cout << "Reached end of file." << std::endl;
		return;
	}
}


bool VMParser::HasMoreCommands()
{
	if (!myAssemblyFile.eof())
	{
		return true;
	}

	return false;

}

void VMParser::GetNextLine()
{
	if (!myAssemblyFile.eof())
	{
		std::getline(myAssemblyFile, currentLine);
		currentLine = trim(currentLine, " \t\n\r\f\v");
	}
}

void VMParser::GetNextCommand()
{
	GetNextLine();

	while (currentLine.find_first_not_of(' ') == string::npos || (currentLine[0] == '/' && currentLine[1] == '/') || (currentLine.empty()))
	{
		if (myAssemblyFile.eof())
		{
			return;
		}
		GetNextLine();

	}

	ParseLine();

}

void VMParser::ParseLine()
{
	size_t whitespace = currentLine.find_first_of(" \t\n\r\f\v");
	std::string currentCommand = currentLine.substr(0, whitespace);

	if (currentCommand == C_PUSH || currentCommand == C_POP)
	{
		if (currentCommand == C_PUSH)
			myCommandType = CommandType::VM_PUSH;
		else if (currentCommand == C_POP)
			myCommandType = CommandType::VM_POP;

		currentLine.erase(currentLine.begin(), currentLine.begin() + whitespace);
		currentLine = trim(currentLine, " \t\n\r\f\v");
		whitespace = currentLine.find_first_of(" \t\n\r\f\v");
		SetArgument1(currentLine.substr(0, whitespace));

		currentLine.erase(currentLine.begin(), currentLine.begin() + whitespace);
		currentLine = trim(currentLine, " \t\n\r\f\v");
		whitespace = currentLine.find_first_of(" \t\n\r\f\v");
		arg2 = currentLine.substr(0, whitespace);
		// TODO: Use another method of splitting the string by whitespace into tokens and setting values to tokens
	}
	else if (currentCommand == C_ADD)
		myCommandType = CommandType::VM_ADD;

	else if (currentCommand == C_SUB)
		myCommandType = CommandType::VM_SUB;

}



void VMParser::WriteToFile(std::ofstream &targetFile)
{
	if (TypeOfCommand() == CommandType::VM_PUSH)
	{
		if (arg1 == "temp")
		{
			targetFile << "@" << 5 + std::stoi(arg2) << std::endl;
			targetFile << "D=M" << std::endl;
		}
		else
		{
			targetFile << "@" << arg2 << std::endl;
			targetFile << "D=A" << std::endl;

			if (arg1 != "constant")
			{
				if (arg1 == "static")
					targetFile << "@" << inputFileName << "." << arg1 << std::endl;
				else
					targetFile << "@" << arg1 << std::endl;

				targetFile << "A=M+D" << std::endl;
				targetFile << "D=M" << std::endl;				
			}
		}

		targetFile << "@SP" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "M=D" << std::endl;

		targetFile << "@SP" << std::endl;
		targetFile << "M=M+1" << std::endl;

		

	}
	else if (TypeOfCommand() == CommandType::VM_POP)
	{
		if (arg1 == "temp")
		{
			targetFile << "@SP" << std::endl;
			targetFile << "M=M-1" << std::endl;
			targetFile << "A=M" << std::endl;
			targetFile << "D=M" << std::endl;

			targetFile << "@" << 5 + std::stoi(arg2) << std::endl;
			targetFile << "M=D" << std::endl;
		}
		else
		{
			targetFile << "@" << arg2 << std::endl;
			targetFile << "D=A" << std::endl;

			if (arg1 == "static")
				targetFile << "@" << inputFileName << "." << arg1 << std::endl;
			else
				targetFile << "@" << arg1 << std::endl;

			targetFile << "D=M+D" << std::endl;
			targetFile << "@R13" << std::endl;
			targetFile << "M=D" << std::endl;

			targetFile << "@SP" << std::endl;
			targetFile << "M=M-1" << std::endl;
			targetFile << "A=M" << std::endl;
			targetFile << "D=M" << std::endl;

			targetFile << "@R13" << std::endl;
			targetFile << "A=M" << std::endl;
			targetFile << "M=D" << std::endl;
		}		
	}
	else if (TypeOfCommand() == CommandType::VM_ADD || TypeOfCommand() == CommandType::VM_SUB)
	{
		targetFile << "@SP" << std::endl;
		targetFile << "M=M-1" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "D=M" << std::endl;
		targetFile << "@SP" << std::endl;
		targetFile << "M=M-1" << std::endl;
		targetFile << "A=M" << std::endl;
		if (TypeOfCommand() == CommandType::VM_ADD)
			targetFile << "M=M+D" << std::endl;
		else if (TypeOfCommand() == CommandType::VM_SUB)
			targetFile << "M=M-D" << std::endl;

		targetFile << "@SP" << std::endl;
		targetFile << "M=M+1" << std::endl;
		
	}
}

void VMParser::Clear()
{
}

CommandType VMParser::TypeOfCommand()
{
	return myCommandType;
}

void VMParser::SetArgument1(std::string arg)
{
	try
	{
		if (RegisterStrings.find(arg) == RegisterStrings.end())
		{
			throw ExceptionHandler();
		}
		else
		{
			arg1 = RegisterStrings[arg];
		}
	}
	catch (ExceptionHandler E)
	{
		E.InvalidVMArgumentSpecified(arg);
	}


}
