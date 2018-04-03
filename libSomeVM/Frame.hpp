#pragma once

#include <cstdint>

#include "Instruction.hpp"
#include "Registry.hpp"
#include "Value.hpp"

namespace svm
{
    class Function;

    class Frame
    {
    public:
        Frame(const Function& function, Register argsIdx, uint16_t nargs);
        ~Frame() = default;

        Bytecode::const_iterator begin() const;
        Bytecode::const_iterator end() const;

        Bytecode::const_iterator next();

        void jump(uint64_t instIdx);

        uint64_t length() const;

    private:
        const Function& function;
        Register argsIdx;
        uint16_t nargs;
        Bytecode::const_iterator currentInstruction;
    };
}
