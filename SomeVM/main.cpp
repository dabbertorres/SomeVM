#include <cstdlib>
#include <iostream>
#include <fstream>

#include "SomeLang/Assembler.hpp"
#include "SomeLang/Lexer.hpp"
#include "SomeLang/Parser.hpp"

#include "libSomeVM/Program.hpp"
#include "libSomeVM/VM.hpp"

#include "Repl.hpp"

void repl();
void run(const char* input);
void build(const char* input, const char* output);

int main(int argc, char** argv) try
{
    switch (argc)
    {
    case 1:
        repl();
        break;

    case 2:
        run(argv[1]);
        break;

    case 3:
        build(argv[1], argv[2]);
        break;

    default:
        throw std::runtime_error{
            R"(Incorrect number of arguments. Must be 0 - 2:
    0 args: repl mode
    1 arg:  binary to execute
    2 args: input file to build, and output file to create)"
        };
        break;
    }

    return EXIT_SUCCESS;
}
catch (const std::exception& e)
{
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
}

void repl()
{
    Repl repl{ std::cin, std::cout };
    repl.run();
}

void run(const char* input)
{
    svm::VM vm;
    svm::Program program;
    std::ifstream fin{ input };

    program.load(fin);
    vm.load(program);
    vm.run();
}

void build(const char* input, const char* output)
{
    std::ifstream fin{ input };
    std::ofstream fout{ output };

    auto tokens = sl::lex(fin);
    auto stmts = sl::parse(input, fin, std::cout, tokens);
    auto program = sl::assemble(stmts);

    program.write(fout);
}
