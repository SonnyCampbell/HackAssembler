#pragma once
#include <map>

typedef enum
{
	NoCommand,
	ACommand,
	CCommand,
	LCommand

} CommandType;

static std::map< CommandType, const char * > CommndStrings = {
	{ ACommand, "A instruction" },
	{ CCommand, "C instruction" },
	{ LCommand, "L instruction" }
};