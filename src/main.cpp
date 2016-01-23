#include <iostream>
#include <fstream>

#include "Assembler.hpp"
#include "VM.hpp"

using namespace dbr;

int main(int argc, char** argv)
{
	svm::VM vm(std::cin, std::cout);

	if(argc == 1)
	{
		vm.repl();
	}
	else
	{
		std::ifstream fin(argv[1]);

		if(!fin)
			return 1;

		try
		{
			svm::Program program = svm::il::Assembler::run(fin, std::cout);

			svm::VM::writeBinary(program, argv[2]);

			vm.run(program);
		}
		catch(const std::exception& e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}

	return 0;
}
