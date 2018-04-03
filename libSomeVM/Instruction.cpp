#include "Instruction.hpp"

#include <map>

namespace svm
{
    constexpr const char* stringMap[] =
    {
        // memory ops
        "load",

        // integer math
        "add",
        "sub",
        "mult",
        "div",
        "mod",
        "neg",

        // floating-point math
        "addf",
        "subf",
        "multf",
        "divf",
        "modf",
        "negf",

        // integer comparison
        "lt",
        "lteq",
        "gt",
        "gteq",
        "eq",
        "neq",

        // floating-point comparison
        "ltf",
        "lteqf",
        "gtf",
        "gteqf",
        "eqf",
        "neqf",

        // boolean logic 
        "not",
        "and",
        "or",
        "xor",

        // conditional branching
        "jmpt",
        "jmpf",

        // branching
        "call",
        "ret",
        "jmp",

        // misc
        "syscall",
        "nop",
    };

    Instruction::Code Instruction::type(const std::string& str)
    {
        for (size_t i = 0; i < static_cast<size_t>(Code::Nop); ++i)
        {
            if (str == stringMap[i])
                return static_cast<Code>(i);
        }

        return Code::Nop;
    }

    Instruction::Instruction()
        : Instruction(0)
    {}

    Instruction::Instruction(uint64_t val)
        : value{ val }
    {}

    Instruction::Instruction(Code t, uint16_t one, uint16_t two, uint16_t three)
        : value{ 0 }
    {
        value = static_cast<uint8_t>(t);
        value <<= 56;

        value |= static_cast<uint64_t>(one) << 32;
        value |= static_cast<uint64_t>(two) << 16;
        value |= three;
    }

    Instruction::Code Instruction::type() const
    {
        return static_cast<Code>(value >> 56 & 0xff);
    }

    uint16_t Instruction::arg1() const
    {
        return static_cast<uint16_t>(value >> 32 & 0xffff);
    }

    uint16_t Instruction::arg2() const
    {
        return static_cast<uint16_t>(value >> 16 & 0xffff);
    }

    uint16_t Instruction::arg3() const
    {
        return static_cast<uint16_t>(value & 0xffff);
    }
}
