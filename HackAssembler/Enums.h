#pragma once
#include <map>

typedef enum
{
	ACommand,
	CCommand,
	LCommand

} CommandType;

static std::map< CommandType, const char * > CommndStrings = {
	{ ACommand, "A instruction" },
	{ CCommand, "C instruction" },
	{ LCommand, "L instruction" }
};