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

    struct StatementIf
    {
        std::string conditionValue;
        std::vector<svm::Instruction> instructions;
    };

    struct StatementSwitch
    {
        std::string conditionValue;
        std::vector<svm::Instruction> instructions;
    };

    struct StatementCase
    {
        std::string conditionValue;
        std::vector<svm::Instruction> instructions;
    };

    struct StatementElse
    {
        std::vector<svm::Instruction> instructions;
    };

    struct StatementWhile
    {
        std::string conditionIdentifier;
        std::vector<svm::Instruction> instructions;
    };

    struct StatementFunction
    {
        std::string identifier;
        size_t args;
        size_t rets;
        std::vector<StatementInstruction> instructions;
    };

    using Statement = std::variant<StatementInstruction, StatementIf, StatementSwitch, StatementCase, StatementElse, StatementWhile, StatementFunction>;
}
