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
	else if (currentCommand == C_IFGOTO || currentCommand == C_GOTO || currentCommand == C_LABEL)
	{
		if(currentCommand == C_IFGOTO)
			myCommandType = CommandType::VM_IFGOTO;
		else if(currentCommand == C_GOTO)
			myCommandType = CommandType::VM_GOTO;
		else if(currentCommand == C_LABEL)
			myCommandType = CommandType::VM_LABEL;

		currentLine.erase(currentLine.begin(), currentLine.begin() + whitespace);
		currentLine = trim(currentLine, " \t\n\r\f\v");
		whitespace = currentLine.find_first_of(" \t\n\r\f\v");
		arg2 = currentLine.substr(0, whitespace);
	}
	else if (VM_ArithemticCommands.find(currentCommand) != VM_ArithemticCommands.end())
		myCommandType = VM_ArithemticCommands[currentCommand];

	else
	{
		std::cout << "Error command: " << currentCommand << std::endl;
		// TODO: Handle error case
	}
		


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
		else if (arg1 == "pointer")
		{
			targetFile << "@" << 3 + std::stoi(arg2) << std::endl;
			targetFile << "D=M" << std::endl;
		}
		else
		{
			targetFile << "@" << arg2 << std::endl;
			targetFile << "D=A" << std::endl;

			if (arg1 != "constant")
			{
				if (arg1 == "static")
					targetFile << "@" << inputFileName << "." << arg2 << std::endl;
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
		else if (arg1 == "pointer")
		{
			targetFile << "@SP" << std::endl;
			targetFile << "M=M-1" << std::endl;
			targetFile << "A=M" << std::endl;
			targetFile << "D=M" << std::endl;

			targetFile << "@" << 3 + std::stoi(arg2) << std::endl;
			targetFile << "M=D" << std::endl;
		}
		else
		{
			targetFile << "@" << arg2 << std::endl;
			targetFile << "D=A" << std::endl;

			if (arg1 == "static")
				targetFile << "@" << inputFileName << "." << arg2 << std::endl;
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
	else if (TypeOfCommand() == CommandType::VM_LABEL)
	{
		targetFile << "(" << arg2 << ")" << std::endl;
	}
	else if (TypeOfCommand() == CommandType::VM_IFGOTO)
	{
		targetFile << "@SP" << std::endl;
		targetFile << "M=M-1" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "D=M" << std::endl;
		targetFile << "@" << arg2 << std::endl;
		targetFile << "D;JNE" << std::endl;
	}
	else if (TypeOfCommand() == CommandType::VM_GOTO)
	{
		targetFile << "@" << arg2 << std::endl;
		targetFile << "0;JMP" << std::endl;
	}
	else
	{
		targetFile << "@SP" << std::endl;
		targetFile << "M=M-1" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "D=M" << std::endl;

		if (TypeOfCommand() != CommandType::VM_NEG && TypeOfCommand() != CommandType::VM_NOT)
		{
			targetFile << "@SP" << std::endl;
			targetFile << "M=M-1" << std::endl;
			targetFile << "A=M" << std::endl;
		}
		

		switch (TypeOfCommand())
		{
		case CommandType::VM_ADD:
			targetFile << "M=M+D" << std::endl;
			break;
		case CommandType::VM_SUB:
			targetFile << "M=M-D" << std::endl;
			break;
		case CommandType::VM_NEG:
			targetFile << "M=-M" << std::endl;
			break;
		case CommandType::VM_NOT:
			targetFile << "M=!M" << std::endl;
			break;

		default:

			switch (TypeOfCommand())
			{
			case CommandType::VM_EQ:
				targetFile << "D=M-D" << std::endl;
				targetFile << "@TRUE" << jumpCount << std::endl;
				targetFile << "D;JEQ" << std::endl;
				break;

			case CommandType::VM_GT:
				targetFile << "D=M-D" << std::endl;
				targetFile << "@TRUE" << jumpCount << std::endl;
				targetFile << "D;JGT" << std::endl;
				break;

			case CommandType::VM_LT:
				targetFile << "D=M-D" << std::endl;
				targetFile << "@TRUE" << jumpCount << std::endl;
				targetFile << "D;JLT" << std::endl;
				break;

			case CommandType::VM_AND:
				targetFile << "M=D&M" << std::endl;
				targetFile << "@CONT" << jumpCount << std::endl;
				targetFile << "0;JMP" << std::endl;
				break;

			case CommandType::VM_OR:
				targetFile << "M=D|M" << std::endl;
				targetFile << "@CONT" << jumpCount << std::endl;
				targetFile << "0;JMP" << std::endl;
				break;

			default:
				break;
			}

			targetFile << "@SP" << std::endl;
			targetFile << "A=M" << std::endl;
			targetFile << "M=0" << std::endl;
			targetFile << "@CONT" << jumpCount << std::endl;
			targetFile << "0;JMP" << std::endl;
			targetFile << "(TRUE" << jumpCount << ")" << std::endl;
			targetFile << "@SP" << std::endl;
			targetFile << "A=M" << std::endl;
			targetFile << "M=-1" << std::endl;
			targetFile << "(CONT" << jumpCount << ")" << std::endl;
			jumpCount++;
			break;

		}


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
