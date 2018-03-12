#pragma once

#include <variant>
#include <vector>
#include <string>

#include "libSomeVM/Value.hpp"

namespace sl
{
    using Literal = std::variant<svm::Float, std::string, bool>;
    using Value = std::variant<Literal, std::string>;

    struct StatementInstruction
    {
        std::string instruction;
        std::vector<Value> values;
    };

    struct StatementIf
    {
        std::string conditionValue;
        std::vector<StatementInstruction> instructions;
    };

    struct StatementSwitch
    {
        std::string conditionValue;
        std::vector<StatementInstruction> instructions;
    };

    struct StatementCase
    {
        std::string conditionValue;
        std::vector<StatementInstruction> instructions;
    };

    struct StatementElse
    {
        std::vector<StatementInstruction> instructions;
    };

    struct StatementWhile
    {
        std::string conditionIdentifier;
        std::vector<StatementInstruction> instructions;
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
