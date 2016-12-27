#pragma once
#include <map>
#include <string>

typedef enum
{
	NoCommand,
	ASM_ACommand,
	ASM_CCommand,
	ASM_LCommand,
	VM_PUSH,
	VM_POP,
	VM_ADD,
	VM_SUB,
	VM_LABEL,
	VM_GOTO,
	VM_IF,
	VM_FUNCTION,
	VM_RETURN,
	VM_CALL

} CommandType;

static std::map< CommandType, const char * > CommndStrings = {
	{ ASM_ACommand, "A instruction" },
	{ ASM_CCommand, "C instruction" },
	{ ASM_LCommand, "L instruction" }
};

static std::map< std::string, std::string > RegisterStrings = {
	{ "local", "LCL" },
	{ "argument", "ARG" },
	{ "this", "THIS" },
	{ "that", "THAT" },
	{ "constant", "constant" },
	{ "pointer", "pointer" },
	{ "temp", "temp" },
	{ "static", "static" }
};