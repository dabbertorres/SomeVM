#pragma once

#include <iosfwd>
#include <string>
#include <unordered_map>
#include <vector>

#include "libSomeVM/Register.hpp"
#include "libSomeVM/Program.hpp"
#include "libSomeVM/Instruction.hpp"

#include "Parser.hpp"
#include "Statement.hpp"

namespace sl
{
    svm::Program assemble(const std::vector<Statement>& stmts);

    class Assembler
    {
    public:
        Assembler() = delete;
        ~Assembler() = delete;

        static svm::Program run(std::istream& in, std::ostream& logStream);

        // adds to program, rather than creating a whole new Program
        static void run(std::istream& in, std::ostream& logStream, svm::Program& program);

        static bool isRegister(const std::string& str);
        static svm::Register toRegister(const std::string& regStr);

    private:
        static const std::unordered_map<std::string, svm::Instruction(*)(std::istream&, svm::Program&)> commands;
    };
}
