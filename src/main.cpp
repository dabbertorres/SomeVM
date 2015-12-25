#include <iostream>
#include <fstream>

#include "Assembler.hpp"
#include "VM.hpp"

using namespace dbr;

int main(int argc, char** argv)
{
	svm::VM vm;

	if(argc == 1)
	{
		vm.repl(std::cin, std::cout);
	}
	else
	{
		std::ifstream fin(argv[1]);

		if(!fin)
			return 1;

		try
		{
			svm::Program program = svm::il::Assembler::run(fin, std::cout);

			svm::VM::writeBinary(program, "tests/helloWorld.svm");

			vm.run(program, std::cin, std::cout);
		}
		catch(const std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		svm::Program program = svm::VM::loadBinary("tests/helloWorld.svm");

		vm.run(program, std::cin, std::cout);
	}

	std::cin.get();
	return 0;
}
