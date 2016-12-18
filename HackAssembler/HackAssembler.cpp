#include "HackParser.h"
#include "MachineCodeTranslate.h"
#include "Enums.h"
#include <iostream>
#include <fstream>

void NoFileError();


int main(int argc, char *argv[])
{
	/*if (argc == 1)
	{
		NoFileError();
	}*/
	

	HackParser parser("Pong.asm");
	std::ofstream machineCodeFile;
	machineCodeFile.open("Pong1.hack");

	parser.Advance();
	while (parser.HasMoreCommands())
	{
		// TODO: Refactor out XXXXToBinary functions into the HackParser Class
		if (parser.TypeOfCommand() == CCommand)
		{
			machineCodeFile << "111" << CompToBinary(parser.Comp()) << DestToBinary(parser.Dest()) << JumpToBinary(parser.Jump()) << std::endl;
		}
		else if (parser.TypeOfCommand() == ACommand)
		{
			machineCodeFile << "0" << DecimalAddressToBinary( parser.Symbol() )<< std::endl;
		}

		parser.Clear();
		parser.Advance();
	}

	machineCodeFile.close();
	std::cout << "File written successfully." << std::endl;

	int n;
	std::cin >> n;
	return 0;
}




void NoFileError()
{
	std::cout << "No file given specified to compile. Please restart and specify an assembly file." << std::endl;
}
