#include "Frame.hpp"

#include "Function.hpp"

namespace svm
{
    Frame::Frame(const Function& function, Register argsIdx, uint16_t nargs)
        : function{ function },
        argsIdx{ argsIdx },
        nargs{ nargs },
        currentInstruction{ function.begin() }
    {}

    Bytecode::const_iterator Frame::next()
    {
        return currentInstruction++;
    }

    void Frame::jump(uint64_t instIdx)
    {
        currentInstruction = function.begin() + instIdx;
    }

    Bytecode::const_iterator Frame::begin() const
    {
        return function.begin();
    }

    Bytecode::const_iterator Frame::end() const
    {
        return function.end();
    }

    uint64_t Frame::length() const
    {
        return function.length();
    }
}