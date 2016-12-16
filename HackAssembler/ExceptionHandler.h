#pragma once
#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <string>



class ExceptionHandler : public std::runtime_error
{
public:
	ExceptionHandler();
	//~ExceptionHandler();

	const char *IncompatibleInstructionError(std::string, std::string);
};

