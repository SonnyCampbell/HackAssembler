#include "HackParser.h"



string DestToBinary(string dest)
{
	std::unordered_map< string, string >::iterator it = DestInstructions.find(dest);
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
	std::unordered_map< string, string >::iterator it = CompInstructions.find(comp);
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
	std::unordered_map< string, string >::iterator it = JumpInstructions.find(jump);
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
