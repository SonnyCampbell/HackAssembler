#include "ExceptionHandler.h"

ExceptionHandler::ExceptionHandler() : std::runtime_error("")
{
	return;
}



const char *ExceptionHandler::IncompatibleInstructionError(std::string commandType, std::string functionName)
{
	std::cout << "Incompatible command type [" << commandType << "] called from " << functionName << "()"<< std::endl;

	return 0;
}

const char *ExceptionHandler::BinaryInstructionNotFound(std::string instruction, std::string functionName)
{
	std::cout << "Instruction [" << instruction << "] has no valid binary form. Called from " << functionName << "()" << std::endl;

	return 0;
}
