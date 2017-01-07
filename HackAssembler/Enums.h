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
	VM_NEG,
	VM_EQ,
	VM_GT,
	VM_LT,
	VM_AND,
	VM_OR,
	VM_NOT,
	VM_LABEL,
	VM_GOTO,
	VM_IF,
	VM_FUNCTION,
	VM_RETURN,
	VM_CALL,
	VM_IFGOTO	

} CommandType;

static std::map< CommandType, const char * > ASM_CommndStrings = {
	{ ASM_ACommand, "A instruction" },
	{ ASM_CCommand, "C instruction" },
	{ ASM_LCommand, "L instruction" }
};

static std::map< std::string, CommandType > VM_ArithemticCommands = {
	{ "add", VM_ADD },
	{ "sub", VM_SUB },
	{ "neg", VM_NEG },
	{ "eq", VM_EQ },
	{ "gt", VM_GT },
	{ "lt", VM_LT },
	{ "and", VM_AND },
	{ "or", VM_OR },
	{ "not", VM_NOT }
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