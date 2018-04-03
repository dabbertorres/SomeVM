#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace svm
{
    /*
        An instruction is encoded in 8 bytes.
        The rest of the bytes are the arguments, stored in 16 bits.

        format (i is instruction type bits, 1, 2, 3 are bits for that arg number, _ are unused bits, | separates bytes):
        i i i i i i i i | _ _ _ _ _ _ _ _ | 1 1 1 1 1 1 1 1 | 1 1 1 1 1 1 1 1 | 2 2 2 2 2 2 2 2 | 2 2 2 2 2 2 2 2 | 3 3 3 3 3 3 3 3 | 3 3 3 3 3 3 3 3
     */
    class Instruction
    {
    public:
        enum class Code : uint8_t
        {
            /* memory ops */
            Load,		// 1: write-to, 2: index

            /* integer math */
            Add,		// "addition" 1: write-to, 2: registry index, 3: registry index
            Sub,		// "subtraction" 1: write-to, 2: registry index, 3: registry index
            Mult,		// "multiplication" 1: write-to, 2: registry index, 3: registry index
            Div,		// "division" 1: write-to, 2: registry index, 3: registry index
            Mod,		// "modulus" 1: write-to, 2: registry index, 3: registry index
            Neg,		// "negative" 1: write-to, 2: registry index

            /* floating-point math */
            Addf,		// "addition" 1: write-to, 2: registry index, 3: registry index
            Subf,		// "subtraction" 1: write-to, 2: registry index, 3: registry index
            Multf,		// "multiplication" 1: write-to, 2: registry index, 3: registry index
            Divf,		// "division" 1: write-to, 2: registry index, 3: registry index
            Modf,		// "modulus" 1: write-to, 2: registry index, 3: registry index
            Negf,		// "negative" 1: write-to, 2: registry index

            /* integer comparison */
            Lt,			// "less than" 1: write-to, 2: registry index, 3: registry index
            LtEq,		// "less than or equal" 1: write-to, 2: registry index, 3: registry index
            Gt,			// "greater than" 1: write-to, 2: registry index, 3: registry index
            GtEq,		// "greater than or equal" 1: write-to, 2: registry index, 3: registry index
            Eq,			// "equals" 1: write-to, 2: registry index, 3: registry index
            Neq,		// "not equals" 1: write-to, 2: registry index, 3: registry index

            /* floating-point comparison */
            Ltf,		// "less than" 1: write-to, 2: registry index, 3: registry index
            LtEqf,		// "less than or equal" 1: write-to, 2: registry index, 3: registry index
            Gtf,		// "greater than" 1: write-to, 2: registry index, 3: registry index
            GtEqf,		// "greater than or equal" 1: write-to, 2: registry index, 3: registry index
            Eqf,		// "equals" 1: write-to, 2: registry index, 3: registry index
            Neqf,		// "not equals" 1: write-to, 2: registry index, 3: registry index

            /* boolean logic */
            Not,		// 1: write-to, 2: registry index
            And,		// 1: write-to, 2: registry index, 3: registry index
            Or,			// 1: write-to, 2: registry index, 3: registry index
            Xor,		// 1: write-to, 2: registry index, 3: registry index

            /* conditional branching */
            JmpT,			// 1: registry index, 2x: instruction index
            JmpF,			// 1: registry index, 2x: instruction index

            /* branching */
            Call,		// 1: number of arguments to call with, 2: registry index of start of arguments, 3: function index
            Ret,		// "return" 1: number of returns, 2: registry index of start of return values
            Jmp,		// 1x: instruction index

            /* misc */
            SysCall,	// 1: number of arguments to call with, 2: registry index of start of arguments, 3: function index
            Nop,		// "No operation". Any arguments are ignored
        };

        static Code type(const std::string& str);

        Instruction();
        Instruction(uint64_t val);
        Instruction(Code t, uint16_t, uint16_t, uint16_t);

        Code type() const;

        uint16_t arg1() const;
        uint16_t arg2() const;
        uint16_t arg3() const;

    private:
        uint64_t value;
    };

    using Bytecode = std::vector<Instruction>;
}
