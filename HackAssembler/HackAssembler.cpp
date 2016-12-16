#include "HackParser.h"
#include "Enums.h"
#include <iostream>

void NoFileError();


int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		NoFileError();
	}
	

	HackParser parser(argv[1]);

	parser.Advance();
	parser.Advance();

	std::cout << parser.Comp() << std::endl;

	parser.Advance();
	

	


	int n;
	std::cin >> n;
	return 0;
}




void NoFileError()
{
	std::cout << "No file given specified to compile. Please restart and specify an assembly file." << std::endl;
}
