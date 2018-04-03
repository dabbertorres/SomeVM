#include "Statement.hpp"

#include <ostream>

using namespace sl;

std::ostream& operator<<(std::ostream& os, const Statement& stmt)
{
    std::visit([&](auto&& arg) { os << arg; }, stmt);
    return os;
}

std::ostream& operator<<(std::ostream& os, const StatementInstruction& stmt)
{
    os << "Instruction: " << stmt.instruction << '\n';

    for (auto& v : stmt.args)
    {
        os << '\t' << v << '\n';
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const StatementIf& stmt)
{
    os << "if (" << stmt.condition << ")\n{\n";

    for (auto& st : stmt.trueBranch)
    {
        os << '\t' << st;
    }

    if (!stmt.falseBranch.empty())
    {
        os << "}\nelse\n{\n";

        for (auto& st : stmt.falseBranch)
        {
            os << '\t' << st;
        }
    }

    os << "}\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const StatementCase& stmt)
{
    os << "case " << stmt.condition << ":\n";

    for (auto& st : stmt.statements)
    {
        os << '\t' << st;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const StatementSwitch& stmt)
{
    os << "switch (" << stmt.condition << ")\n{\n";

    for (auto& c : stmt.cases)
    {
        os << c;
    }

    if (!stmt.elseStatements.empty())
    {
        os << "}\nelse\n{\n";

        for (auto& st : stmt.elseStatements)
        {
            os << st;
        }
    }

    os << "}\n";

    return os;
}

std::ostream& operator<<(std::ostream& os, const StatementWhile& stmt)
{
    os << "while (" << stmt.condition << ")\n{\n";

    for (auto& st : stmt.statements)
    {
        os << st;
    }

    os << "}\n";

    return os;
}

std::ostream& operator<<(std::ostream& os, const StatementFunction& stmt)
{
    os << "func " << stmt.identifier << "( ";

    for (auto& a : stmt.args)
    {
        os << a.identifier << " ";
    }

    os << ") > ( ";

    for (auto& r : stmt.rets)
    {
        os << r.identifier << " ";
    }

    os << ")\n{\n";

    for (auto& st : stmt.statements)
    {
        os << st;
    }

    os << "}\n";
    
    return os;
}

std::ostream& operator<<(std::ostream& os, const Value& value)
{
    switch (value.index())
    {
    case 0:
        {
            auto& sv = std::get<svm::Value>(value);

            switch (sv.index())
            {
            case svm::typeIndex<svm::Bool>():
                os << std::boolalpha << std::get<svm::Bool>(sv);
                break;

            case svm::typeIndex<svm::Int>():
                os << std::get<svm::Int>(sv);
                break;

            case svm::typeIndex<svm::Float>():
                os << std::get<svm::Float>(sv);
                break;

            case svm::typeIndex<svm::String>():
            {
                auto& str = std::get<svm::String>(sv);
                os.write(str.data(), str.length());
                break;
            }

            default:
                os << "unknown value";
                break;
            }
        }
        break;

    case 1:
        os << "var " << std::get<Variable>(value).identifier;
        break;

    default:
        os << "unknown";
        break;
    }

    return os;
}
