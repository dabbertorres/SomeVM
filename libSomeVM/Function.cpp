#include "Function.hpp"

namespace svm
{
    Function::Function(uint8_t nrets, uint8_t nargs, Bytecode code)
        : nrets(nrets),
        nargs(nargs),
        code(code)
    {}

    Function::Function(const Function& other)
        : nrets(other.nrets),
        nargs(other.nargs),
        code(other.code)
    {}

    Function::Function(Function&& other)
        : nrets(other.nrets),
        nargs(other.nargs),
        code(std::move(other.code))
    {}

    Function& Function::operator=(const Function& other)
    {
        nrets = other.nrets;
        nargs = other.nargs;
        code = other.code;

        return *this;
    }

    Function& Function::operator=(Function&& other)
    {
        nrets = other.nrets;
        nargs = other.nargs;
        code = std::move(other.code);

        return *this;
    }

    Bytecode::const_iterator Function::begin() const
    {
        return code.begin();
    }

    Bytecode::const_iterator Function::end() const
    {
        return code.end();
    }

    uint64_t Function::length() const
    {
        return code.size();
    }

    const Bytecode& Function::bytecode() const
    {
        return code;
    }

    uint8_t Function::returns() const
    {
        return nrets;
    }

    uint8_t Function::args() const
    {
        return nargs;
    }
}
