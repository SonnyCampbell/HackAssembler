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
	return "";
}
string HackParser::Dest()
{
	return "";
}
string HackParser::Comp()
{
	return "";
}
string HackParser::Jump()
{
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

void HackParser::ParseLine()
{
	if (currentLine[0] == ACOMMAND)
	{
		size_t whitespace = currentLine.find_first_of(' ');
		myAInstruction = currentLine.substr(1, whitespace-1);
		
		myCommandType = ACommand;
	}
	else
	{
		ParseCInstruction();
		myCommandType = CCommand;
	}

}

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

		size_t whitespace = currentLine.find_first_of(' ');
		myJumpInstruction = currentLine.substr(0, whitespace);		
	}
	else
	{
		size_t whitespace = currentLine.find_first_of(' ');
		myCompInstruction = currentLine.substr(0, whitespace);
	}

	currentLine.erase(currentLine.begin(), currentLine.end());
	
}

void HackParser::ParseLabels()
{
	if (currentLine[0] == LCOMMAND_OPEN)
	{
		size_t closePosition = currentLine.find_first_of(LCOMMAND_CLOSE);
		myLoopInstruction = currentLine.substr(1, closePosition - 1);
		myCommandType = LCommand;
	}
}
