#include "HackParser.h"


HackParser::HackParser(string filename)
{
	myAssemblyFile.open(filename);
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
		if (myCommandType = ACommand)
		{
			return myAInstruction;
		}
		else if (myCommandType = LCommand)
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
		E.IncompatibleInstructionError(CommndStrings[myCommandType], "Symbol");
	}

	return "";
	
}
string HackParser::Dest()
{
	try
	{
		if (myCommandType = CCommand)
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
		E.IncompatibleInstructionError(CommndStrings[myCommandType], "Dest");
	}

	return "";
}
string HackParser::Comp()
{
	try
	{
		if (myCommandType = CCommand)
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
		E.IncompatibleInstructionError(CommndStrings[myCommandType], "Comp");
	}

	return "";
}
string HackParser::Jump()
{
	try
	{
		if (myCommandType = CCommand)
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
		E.IncompatibleInstructionError(CommndStrings[myCommandType], "Jump");
	}

	return "";

}




// trim from left
string& HackParser::ltrim(string& s, const char* t)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from right
string& HackParser::rtrim(string& s, const char* t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from left & right
string& HackParser::trim(string& s, const char* t)
{
	return ltrim(rtrim(s, t), t);
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

	while (currentLine.find_first_not_of(' ') == string::npos)
	{
		GetNextLine();
	}

	while (currentLine[0] == '/' && currentLine[1] == '/')
	{
		GetNextLine();
	}

	if (currentLine.empty())
	{
		return;
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
		
		myCommandType = ACommand;
	}
	else
	{
		ParseCInstruction();
		myCommandType = CCommand;
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
		myLoopInstruction = currentLine.substr(1, closePosition);
		myCommandType = LCommand;
	}
	currentLine.erase(currentLine.begin(), currentLine.end());
}
