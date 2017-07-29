#include <iostream>
#include <fstream>

#include "libSomeVM/VM.hpp"
#include "libSomeVM/Program.hpp"

int main(int argc, char** argv) try
{
    if (argc == 2)
    {
        svm::Program program;
        std::ifstream fin{ argv[1] };
        auto bytes = program.load(fin);

        std::cout << "Loaded " << bytes << " bytes.\n";

        svm::VM vm;
        vm.load(program);
        vm.run();
    }
    else
    {
        std::cout << "Incorrect number of arguments. Must be 0 - 2\n";
        std::cout << "0 args: repl mode\n";
        std::cout << "1 arg: binary to execute\n";
        std::cout << "2 args: input file to assemble, and output file to create\n";
    }

    std::cout << "Press <Enter> to continue...";
    std::cin.get();

    return 0;
}
catch (const std::exception& e)
{
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
}
