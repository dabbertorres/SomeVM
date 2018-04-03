#pragma once

#include <iosfwd>

#include "SomeLang/Assembler.hpp"

#include "libSomeVM/Program.hpp"
#include "libSomeVM/VM.hpp"

class Repl
{
public:
    Repl(std::istream& input, std::ostream& output);
    ~Repl();

    void run();

private:
    svm::VM vm;
    svm::Program program;
    std::istream& input;
    std::ostream& output;
};
