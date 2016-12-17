#pragma once
#include <string>
#include <bitset>
#include <map>
#include "ExceptionHandler.h"

using std::string;

static std::map< string, string > JumpInstructions = {
	{ ""   , "000" },
	{ "JGT", "001" },
	{ "JEQ", "010" },
	{ "JGE", "011" },
	{ "JLT", "100" },
	{ "JNE", "101" },
	{ "JLE", "110" },
	{ "JMP", "111" }
};

static std::map< string, string > DestInstructions = {
	{ ""   , "000" },
	{ "M"  , "001" },
	{ "D"  , "010" },
	{ "MD" , "011" },
	{ "A"  , "100" },
	{ "AM" , "101" },
	{ "AD" , "110" },
	{ "AMD", "111" }
};

static std::map< string, string > CompInstructions = {
	{ "0"  , "0101010" },
	{ "1"  , "0111111" },
	{ "-1" , "0111010" },
	{ "D"  , "0001100" },
	{ "A"  , "0110000" },
	{ "!D" , "0001101" },
	{ "!A" , "0110001" },
	{ "-D" , "0001111" },
	{ "-A" , "0110011" },
	{ "D+1", "0011111" },
	{ "A+1", "0110111" },
	{ "D-1", "0001110" },
	{ "A-1", "0110010" },
	{ "D+A", "0000010" },
	{ "A+D", "0000010" },
	{ "D-A", "0010011" },
	{ "A-D", "0000111" },
	{ "D&A", "0000000" },
	{ "D|A", "0010101" },
	{ "M"  , "1110000" },
	{ "!M" , "1110001" },
	{ "-M" , "1110011" },
	{ "M+1", "1110111" },
	{ "M-1", "1110010" },
	{ "D+M", "1000010" },
	{ "D-M", "1010011" },
	{ "M-D", "1000111" },
	{ "D&M", "1000000" },
	{ "D|M", "1010101" }
};

string DestToBinary(string dest)
{	
	std::map< string, string >::iterator it = DestInstructions.find(dest);
	try
		{
		if (it != DestInstructions.end())
		{
			return  DestInstructions.find(dest)->second;
		}
		else
		{
			throw ExceptionHandler();
		}
	}
	catch (ExceptionHandler E)
	{
		E.BinaryInstructionNotFound(dest, "DestToBinary");
	}
	return "";
	
}

string CompToBinary(string comp)
{
	std::map< string, string >::iterator it = CompInstructions.find(comp);
	try
	{
		if (it != CompInstructions.end())
		{
			return  CompInstructions.find(comp)->second;
		}
		else
		{
			throw ExceptionHandler();
		}
	}
	catch (ExceptionHandler E)
	{
		E.BinaryInstructionNotFound(comp, "CompToBinary");
	}
	return "";
}

string JumpToBinary(string jump)
{
	std::map< string, string >::iterator it = JumpInstructions.find(jump);
	try
	{
		if (it != JumpInstructions.end())
		{
			return  JumpInstructions.find(jump)->second;
		}
		else
		{
			throw ExceptionHandler();
		}
	}
	catch (ExceptionHandler E)
	{
		E.BinaryInstructionNotFound(jump, "JumpToBinary");
	}
	
	return "";
}

string DecimalAddressToBinary(string address)
{
	return std::bitset<15>(std::stoi(address)).to_string();
}

