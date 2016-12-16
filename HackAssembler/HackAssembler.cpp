#include "Parser.h"
#include <iostream>

void NoFileError();



int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		NoFileError();
		return 0;
	}
	
	Parser parser(argv[1]);

	parser.Advance();
	parser.Advance();

	


	int n;
	std::cin >> n;
	return 0;
}


void NoFileError()
{
	std::cout << "No file given specified to compile. Please restart and specify an assembly file." << std::endl;
}
