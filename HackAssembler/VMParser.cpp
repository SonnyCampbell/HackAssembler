#include "VMParser.h"

VMParser::VMParser(string filename)
{

	myAssemblyFile.open(filename);

	size_t fileExt = filename.find_last_of(".");
	size_t lastSlash = filename.find_last_of("/");
	inputFileName = filename.substr(lastSlash + 1, fileExt - lastSlash - 1);
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
		SetRegisterArgument(currentLine.substr(0, whitespace));

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
		label = currentLine.substr(0, whitespace);
	}
	else if (currentCommand == C_FUNCTION)
	{
		myCommandType = CommandType::VM_FUNCTION;

		currentLine.erase(currentLine.begin(), currentLine.begin() + whitespace);
		currentLine = trim(currentLine, " \t\n\r\f\v");
		whitespace = currentLine.find_first_of(" \t\n\r\f\v");
		label = currentLine.substr(0, whitespace);

		currentLine.erase(currentLine.begin(), currentLine.begin() + whitespace);
		currentLine = trim(currentLine, " \t\n\r\f\v");
		whitespace = currentLine.find_first_of(" \t\n\r\f\v");
		arg2 = currentLine.substr(0, whitespace);
	
		//functionName.pop();
		functionName.push(label);
	}
	else if (currentCommand == C_RETURN)
	{
		myCommandType = CommandType::VM_RETURN;

	}
	else if (currentCommand == C_CALL)
	{
		myCommandType = CommandType::VM_CALL;

		currentLine.erase(currentLine.begin(), currentLine.begin() + whitespace);
		currentLine = trim(currentLine, " \t\n\r\f\v");
		whitespace = currentLine.find_first_of(" \t\n\r\f\v");
		label = currentLine.substr(0, whitespace);

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

void VMParser::WriteInit(std::ofstream &targetFile)
{
	targetFile << "@256" << std::endl;
	targetFile << "D=A" << std::endl;
	targetFile << "@SP" << std::endl;
	targetFile << "M=D" << std::endl;


	targetFile << "@Sys.init_return" << std::endl;


	targetFile << "D=A" << std::endl;
	targetFile << "@SP" << std::endl;
	targetFile << "A=M" << std::endl;
	targetFile << "M=D" << std::endl;

	targetFile << "@SP" << std::endl;
	targetFile << "M=M+1" << std::endl;

	targetFile << "@LCL" << std::endl; // push LCL
	targetFile << "D=M" << std::endl;
	targetFile << "@SP" << std::endl;
	targetFile << "A=M" << std::endl;
	targetFile << "M=D" << std::endl;

	targetFile << "@SP" << std::endl;
	targetFile << "M=M+1" << std::endl;

	targetFile << "@ARG" << std::endl; // push ARG
	targetFile << "D=M" << std::endl;
	targetFile << "@SP" << std::endl;
	targetFile << "A=M" << std::endl;
	targetFile << "M=D" << std::endl;

	targetFile << "@SP" << std::endl;
	targetFile << "M=M+1" << std::endl;

	targetFile << "@THIS" << std::endl; // push THIS
	targetFile << "D=M" << std::endl;
	targetFile << "@SP" << std::endl;
	targetFile << "A=M" << std::endl;
	targetFile << "M=D" << std::endl;

	targetFile << "@SP" << std::endl;
	targetFile << "M=M+1" << std::endl;

	targetFile << "@THAT" << std::endl;// push THAT
	targetFile << "D=M" << std::endl;
	targetFile << "@SP" << std::endl;
	targetFile << "A=M" << std::endl;
	targetFile << "M=D" << std::endl;

	targetFile << "@SP" << std::endl;
	targetFile << "M=M+1" << std::endl;

	targetFile << "@0" << std::endl;// ARG = SP-n-5
	targetFile << "D=A" << std::endl;
	targetFile << "@5" << std::endl;
	targetFile << "D=D+A" << std::endl;

	targetFile << "@SP" << std::endl;
	targetFile << "D=M-D" << std::endl;
	targetFile << "@ARG" << std::endl;
	targetFile << "M=D" << std::endl;

	targetFile << "@SP" << std::endl; // LCL = SP
	targetFile << "D=M" << std::endl;
	targetFile << "@LCL" << std::endl;
	targetFile << "M=D" << std::endl;

	targetFile << "@Sys.init" << std::endl;
	targetFile << "0;JMP" << std::endl;

	targetFile << "(Sys.init_return)" << std::endl;

}

void VMParser::WriteToFile(std::ofstream &targetFile)
{
	if (TypeOfCommand() == CommandType::VM_PUSH)
	{
		if (registerArg == "temp")
		{
			targetFile << "@" << 5 + std::stoi(arg2) << std::endl;
			targetFile << "D=M" << std::endl;
		}
		else if (registerArg == "pointer")
		{
			targetFile << "@" << 3 + std::stoi(arg2) << std::endl;
			targetFile << "D=M" << std::endl;
		}
		else
		{
			targetFile << "@" << arg2 << std::endl;
			targetFile << "D=A" << std::endl;

			if (registerArg != "constant")
			{
				if (registerArg == "static")
					targetFile << "@" << inputFileName << "." << arg2 << std::endl;
				else
					targetFile << "@" << registerArg << std::endl;

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
		if (registerArg == "temp")
		{
			targetFile << "@SP" << std::endl;
			targetFile << "M=M-1" << std::endl;
			targetFile << "A=M" << std::endl;
			targetFile << "D=M" << std::endl;

			targetFile << "@" << 5 + std::stoi(arg2) << std::endl;
			targetFile << "M=D" << std::endl;
		}
		else if (registerArg == "pointer")
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

			if (registerArg == "static")
				targetFile << "@" << inputFileName << "." << arg2 << std::endl;
			else
				targetFile << "@" << registerArg << std::endl;

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
		targetFile << "(" << functionName.top() << "$" << label << ")" << std::endl;
	}
	else if (TypeOfCommand() == CommandType::VM_IFGOTO)
	{
		targetFile << "@SP" << std::endl;
		targetFile << "M=M-1" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "D=M" << std::endl;
		targetFile << "@" << functionName.top() << "$" << label << std::endl;
		targetFile << "D;JNE" << std::endl;
	}
	else if (TypeOfCommand() == CommandType::VM_GOTO)
	{
		targetFile << "@" << functionName.top() << "$" << label << std::endl;
		targetFile << "0;JMP" << std::endl;
	}
	else if (TypeOfCommand() == CommandType::VM_FUNCTION)
	{
		targetFile << "(" << label << ")" << std::endl;

		for (int i = 0; i < std::stoi(arg2); i++)
		{
			targetFile << "@0" << std::endl;
			targetFile << "D=A" << std::endl;
			targetFile << "@SP" << std::endl;
			targetFile << "A=M" << std::endl;
			targetFile << "M=D" << std::endl;

			targetFile << "@SP" << std::endl;
			targetFile << "M=M+1" << std::endl;
		}
		
	}
	else if (TypeOfCommand() == CommandType::VM_RETURN)
	{
		targetFile << "@LCL" << std::endl;
		targetFile << "D=M" << std::endl;

		targetFile << "@FRAME" << std::endl;
		targetFile << "M=D" << std::endl; // FRAME = LCL

		targetFile << "@5" << std::endl;
		targetFile << "D=D-A" << std::endl;
		targetFile << "A=D" << std::endl;
		targetFile << "D=M" << std::endl;
		targetFile << "@RET" << std::endl;
		targetFile << "M=D" << std::endl; // RET = *(FRAME - 5)

		targetFile << "@SP" << std::endl;
		targetFile << "M=M-1" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "D=M" << std::endl;
		
		targetFile << "@ARG" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "M=D" << std::endl;
		targetFile << "@ARG" << std::endl;
		targetFile << "D=M" << std::endl; // ARG = pop()
		targetFile << "@SP" << std::endl;
		targetFile << "M=D+1" << std::endl; //SP = ARG + 1

		targetFile << "@FRAME" << std::endl;
		targetFile << "D=M" << std::endl;

		targetFile << "A=D-1" << std::endl;
		targetFile << "D=M" << std::endl;
		targetFile << "@THAT" << std::endl;
		targetFile << "M=D" << std::endl; // THAT = *(FRAME - 1) 

		targetFile << "@FRAME" << std::endl;
		targetFile << "D=M" << std::endl;
		targetFile << "@2" << std::endl;
		targetFile << "A=D-A" << std::endl;

		targetFile << "D=M" << std::endl;
		targetFile << "@THIS" << std::endl;
		targetFile << "M=D" << std::endl;// THIS = *(FRAME - 2)  
	
		targetFile << "@FRAME" << std::endl;
		targetFile << "D=M" << std::endl;
		targetFile << "@3" << std::endl;
		targetFile << "A=D-A" << std::endl;

		targetFile << "D=M" << std::endl;
		targetFile << "@ARG" << std::endl;
		targetFile << "M=D" << std::endl; // ARG = *(FRAME - 3) 

		targetFile << "@FRAME" << std::endl;
		targetFile << "D=M" << std::endl;
		targetFile << "@4" << std::endl;
		targetFile << "A=D-A" << std::endl;

		targetFile << "D=M" << std::endl;
		targetFile << "@LCL" << std::endl;
		targetFile << "M=D" << std::endl; // LCL = *(FRAME - 4) 
		
		targetFile << "@RET" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "0;JMP" << std::endl;
		
	}
	else if (TypeOfCommand() == VM_CALL)
	{
		//push return address

		targetFile << "@" << functionName.top() << "$" << label << "_return" << jumpCount << std::endl;

		
		targetFile << "D=A" << std::endl;
		targetFile << "@SP" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "M=D" << std::endl;

		targetFile << "@SP" << std::endl;
		targetFile << "M=M+1" << std::endl;

		targetFile << "@LCL" << std::endl; // push LCL
		targetFile << "D=M" << std::endl;
		targetFile << "@SP" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "M=D" << std::endl;

		targetFile << "@SP" << std::endl;
		targetFile << "M=M+1" << std::endl;
		
		targetFile << "@ARG" << std::endl; // push ARG
		targetFile << "D=M" << std::endl;
		targetFile << "@SP" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "M=D" << std::endl;

		targetFile << "@SP" << std::endl;
		targetFile << "M=M+1" << std::endl;	
		
		targetFile << "@THIS" << std::endl; // push THIS
		targetFile << "D=M" << std::endl;
		targetFile << "@SP" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "M=D" << std::endl;

		targetFile << "@SP" << std::endl;
		targetFile << "M=M+1" << std::endl;
	
		targetFile << "@THAT" << std::endl;// push THAT
		targetFile << "D=M" << std::endl;
		targetFile << "@SP" << std::endl;
		targetFile << "A=M" << std::endl;
		targetFile << "M=D" << std::endl;

		targetFile << "@SP" << std::endl;
		targetFile << "M=M+1" << std::endl;
		
		targetFile << "@" << arg2 << std::endl;// ARG = SP-n-5
		targetFile << "D=A" << std::endl;
		targetFile << "@5" << std::endl;
		targetFile << "D=D+A" << std::endl;

		targetFile << "@SP" << std::endl;
		targetFile << "D=M-D" << std::endl;		
		targetFile << "@ARG" << std::endl;
		targetFile << "M=D" << std::endl;

		targetFile << "@SP" << std::endl; // LCL = SP
		targetFile << "D=M" << std::endl;
		targetFile << "@LCL" << std::endl;
		targetFile << "M=D" << std::endl;
		
		targetFile << "@" << label << std::endl;
		targetFile << "0;JMP" << std::endl;

		targetFile << "(" << functionName.top() << "$" << label << "_return" << jumpCount << ")" << std::endl;
		jumpCount++;

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

void VMParser::SetRegisterArgument(std::string arg)
{
	try
	{
		if (RegisterStrings.find(arg) == RegisterStrings.end())
		{
			throw ExceptionHandler();
		}
		else
		{
			registerArg = RegisterStrings[arg];
		}
	}
	catch (ExceptionHandler E)
	{
		E.InvalidVMArgumentSpecified(arg);
	}


}
