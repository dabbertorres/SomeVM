#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Lexer.hpp"
#include "Token.hpp"
#include "Parser.hpp"
#include "Assembler.hpp"

int main(int argc, char** argv) try
{
    // argv[1] is a filename to compile
    if (argc < 1)
        throw std::runtime_error("Not enough arguments, must provide a filename to compile.");

    std::ifstream fin{ argv[1] };

    auto tokens = sl::lex(fin);

    for (auto& t : tokens)
        std::cout << t << '\n';

    auto stmts = sl::parse(argv[1], fin, tokens);

    return 0;
}
catch (const std::exception& ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
