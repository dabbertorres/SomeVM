#pragma once

#include <iosfwd>
#include <string>
#include <variant>
#include <vector>

#include "libSomeVM/Value.hpp"

#include "Token.hpp"

namespace sl
{
    struct StatementInstruction;
    struct StatementIf;
    struct StatementCase;
    struct StatementSwitch;
    struct StatementWhile;
    struct StatementFunction;

    using Statement = std::variant<StatementInstruction, StatementIf, StatementSwitch, StatementWhile, StatementFunction>;

    struct Variable
    {
        std::string identifier;
    };

    using Value = std::variant<svm::Value, Variable>;

    struct StatementInstruction
    {
        std::string instruction;
        std::vector<Value> args;
    };

    struct StatementIf
    {
        Value condition;
        std::vector<Statement> trueBranch;
        std::vector<Statement> falseBranch;
    };

    struct StatementCase
    {
        Value condition;
        std::vector<Statement> statements;
    };

    struct StatementSwitch
    {
        Value condition;
        std::vector<StatementCase> cases;
        std::vector<Statement> elseStatements;
    };

    struct StatementWhile
    {
        Value condition;
        std::vector<Statement> statements;
    };

    struct StatementFunction
    {
        std::string identifier;
        std::vector<Variable> args;
        std::vector<Variable> rets;
        std::vector<Statement> statements;
    };
}

std::ostream& operator<<(std::ostream& os, const sl::Statement& stmt);
std::ostream& operator<<(std::ostream& os, const sl::StatementInstruction& stmt);
std::ostream& operator<<(std::ostream& os, const sl::StatementIf& stmt);
std::ostream& operator<<(std::ostream& os, const sl::StatementCase& stmt);
std::ostream& operator<<(std::ostream& os, const sl::StatementSwitch& stmt);
std::ostream& operator<<(std::ostream& os, const sl::StatementWhile& stmt);
std::ostream& operator<<(std::ostream& os, const sl::StatementFunction& stmt);

std::ostream& operator<<(std::ostream& os, const sl::Value& value);
