#pragma once

#include <variant>
#include <string>

#include "libSomeVM/Instruction.hpp"

namespace sl
{
    struct StatementInstruction
    {
        svm::Instruction::Type instruction;
        std::vector<std::string> values;
    };

    struct StatementFunction
    {
        std::string name;
        size_t args;
        size_t rets;
        std::vector<StatementInstruction> instructions;
    };

    using Statement = std::variant<StatementInstruction, StatementFunction>;
}
