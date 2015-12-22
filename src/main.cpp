#include <iostream>
#include <fstream>

#include "Parser.hpp"
#include "VM.hpp"

using namespace dbr;

int main(int argc, char** argv)
{
	svm::VM vm;

	if(argc == 1)
	{
		vm.interpreter(std::cin, std::cout);
	}
	else
	{
		std::ifstream fin(argv[1]);

		svm::Program program = svm::il::Parser::run(fin, std::cout);

		vm.run(program, std::cin, std::cout);
	}

	return 0;
}
