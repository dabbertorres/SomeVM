#include <iostream>
#include <fstream>

#include "Assembler.hpp"
#include "VM.hpp"

int main(int argc, char** argv) try
{
	using namespace dbr;

	svm::VM vm(std::cin, std::cout);

	switch(argc)
	{
		// 1 filename, a binary file to execute
		case 2:
		{
			svm::Program program;
			auto bytes = program.load(argv[1]);

			std::cout << "Loaded " << bytes << " bytes.\n";

			vm.load(program);
			vm.run();

			break;
		}

		// 2 filenames, input code file (1), and output binary file (2)
		case 3:
		{
			std::ifstream fin(argv[1]);
			auto program = svm::Assembler::run(fin, std::cout);
			auto bytes = program.write(argv[2]);

			std::cout << "Wrote " << bytes << " bytes.\n";

			break;
		}

		default:
			std::cout << "Incorrect number of arguments. Must be 0 - 2\n";
			std::cout << "0 args: repl mode\n";
			std::cout << "1 arg: binary to execute\n";
			std::cout << "2 args: input file to assemble, and output file to create\n";

			break;
	}

	std::cout << "Press <Enter> to continue...";
	std::cin.get();

	return 0;
}
catch(const std::exception& e)
{
	std::cout << "Exception: " << e.what() << std::endl;
	return 1;
}
