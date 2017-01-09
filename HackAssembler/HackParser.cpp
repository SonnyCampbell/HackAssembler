#include "HackParser.h"


HackParser::HackParser(string filename)
{
	myAssemblyFile.open(filename);
	firstPass = true;
	lineNumber = 0;
	RAMSymbolNumber = 16;
	InitialiseSymbolTable();
}

void HackParser::InitialiseSymbolTable()
{
	SymbolTable = {
		{ "SP"  , "0" },
		{ "LCL"  , "1" },
		{ "ARG" , "2" },
		{ "THIS"  , "3" },
		{ "THAT"  , "4" },
		{ "SCREEN" , "16384" },
		{ "KBD" , "24576" },
		{ "R0" , "0" },
		{ "R1" , "1" },
		{ "R2", "2" },
		{ "R3", "3" },
		{ "R4", "4" },
		{ "R5", "5" },
		{ "R6", "6" },
		{ "R7", "7" },
		{ "R8", "8" },
		{ "R9", "9" },
		{ "R10","10" },
		{ "R11","11" },
		{ "R12","12" },
		{ "R13","13" },
		{ "R14","14" },
		{ "R15","15" }
	};

}



void HackParser::Advance()
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


bool HackParser::HasMoreCommands()
{
	if (!myAssemblyFile.eof())
	{
		return true;
	}
	else
	{
		if (firstPass)
		{
			myAssemblyFile.clear();
			myAssemblyFile.seekg(0, std::ios::beg);
			firstPass = false;
			return true;
		}
	}
	return false;

}

CommandType HackParser::TypeOfCommand()
{
	return myCommandType;
	
}

string HackParser::Symbol()
{
	try
	{
		if (myCommandType == ASM_ACommand)
		{
			return myAInstruction;
		}
		else if (myCommandType == ASM_LCommand)
		{
			return myLoopInstruction;
		}
		else
		{
			throw ExceptionHandler();
		}
	}
	catch (ExceptionHandler E)
	{
		E.IncompatibleInstructionError(ASM_CommndStrings[myCommandType], "Symbol");
	}

	return "";
	
}
string HackParser::Dest()
{
	try
	{
		if (myCommandType = ASM_CCommand)
		{
			return myDestInstruction;
		}
		else
		{
			throw ExceptionHandler();
		}
	}
	catch (ExceptionHandler E)
	{
		E.IncompatibleInstructionError(ASM_CommndStrings[myCommandType], "Dest");
	}

	return "";
}
string HackParser::Comp()
{
	try
	{
		if (myCommandType = ASM_CCommand)
		{
			return myCompInstruction;
		}
		else
		{
			throw ExceptionHandler();
		}
	}
	catch (ExceptionHandler E)
	{
		E.IncompatibleInstructionError(ASM_CommndStrings[myCommandType], "Comp");
	}

	return "";
}
string HackParser::Jump()
{
	try
	{
		if (myCommandType = ASM_CCommand)
		{
			return myJumpInstruction;
		}
		else
		{
			throw ExceptionHandler();
		}
	}
	catch (ExceptionHandler E)
	{
		E.IncompatibleInstructionError(ASM_CommndStrings[myCommandType], "Jump");
	}

	return "";

}

void HackParser::Clear()
{
	myAInstruction = "";
	myJumpInstruction = "";
	myDestInstruction = "";
	myCompInstruction = "";
	myLoopInstruction = "";
}







void HackParser::GetNextLine()
{
	if (!myAssemblyFile.eof())
	{
		std::getline(myAssemblyFile, currentLine);
		currentLine = trim(currentLine, " \t\n\r\f\v");
	}
}

void HackParser::GetNextCommand()
{
	GetNextLine();

	while (currentLine.find_first_not_of(' ') == string::npos || (currentLine[0] == '/' && currentLine[1] == '/') ||
				 (currentLine.empty()) || (!firstPass && currentLine[0] == '('))
	{
		if (myAssemblyFile.eof())
		{
			return;
		}
		GetNextLine();

	}

	if (!firstPass)
	{
		ParseLine();
	}
	else
	{
		ParseLabels();
	}

	
}

// Parse the currently held line into its instructions
// For now it simply chops off anything after the first instrction
void HackParser::ParseLine()
{
	if (currentLine[0] == ACOMMAND)
	{
		size_t whitespace = currentLine.find_first_of(" \t\n\r\f\v");
		myAInstruction = currentLine.substr(1, whitespace);

		if (!IsNumber(myAInstruction))
		{
			std::unordered_map<string, string>::iterator it = SymbolTable.find(myAInstruction);
			if (it == SymbolTable.end())
			{
				SymbolTable.insert(std::pair<string, string>(myAInstruction, std::to_string(RAMSymbolNumber)));
				myAInstruction = std::to_string(RAMSymbolNumber);
				RAMSymbolNumber++;
			}
			else
			{
				myAInstruction = it->second;
			}
		}
		
		myCommandType = ASM_ACommand;
	}
	else
	{
		ParseCInstruction();
		myCommandType = ASM_CCommand;
	}

}


// Break C Instruction into its dest=comp;jump bits
void HackParser::ParseCInstruction()
{
	size_t destBreak = currentLine.find_first_of("=");
	if (destBreak != string::npos)
	{
		myDestInstruction = currentLine.substr(0, destBreak);
		currentLine.erase(0, destBreak + 1);
	}

	size_t compBreak = currentLine.find_first_of(";");
	if (compBreak != string::npos)
	{
		myCompInstruction = currentLine.substr(0, compBreak);
		currentLine.erase(0, compBreak + 1);

		size_t whitespace = currentLine.find_first_of(" \t\n\r\f\v");
		myJumpInstruction = currentLine.substr(0, whitespace);		
	}
	else
	{
		size_t whitespace = currentLine.find_first_of(" \t\n\r\f\v");
		myCompInstruction = currentLine.substr(0, whitespace);
	}

	currentLine.erase(currentLine.begin(), currentLine.end());
	
}


// Parse Label instructions
void HackParser::ParseLabels()
{
	if (currentLine[0] == LCOMMAND_OPEN)
	{
		size_t closePosition = currentLine.find_first_of(LCOMMAND_CLOSE);
		myLoopInstruction = currentLine.substr(1, closePosition-1);
		myCommandType = ASM_LCommand;

		// TODO: lineNumber + 1 will break if there is a comment or space between Label and next command
		SymbolTable.insert(std::pair<string, string>(myLoopInstruction, std::to_string(lineNumber)));
	}
	else
	{
		lineNumber++;
	}
	currentLine.erase(currentLine.begin(), currentLine.end());
}


void HackParser::WriteToFile(std::ofstream &machineCodeFile)
{
	if (TypeOfCommand() == ASM_CCommand)
	{
		machineCodeFile << "111" << CompToBinary(Comp()) << DestToBinary(Dest()) << JumpToBinary(Jump()) << std::endl;
	}
	else if (TypeOfCommand() == ASM_ACommand)
	{
		machineCodeFile << "0" << DecimalAddressToBinary(Symbol()) << std::endl;
	}
}

void HackParser::WriteInit(std::ofstream &)
{
}
