#pragma once

#include <cstdint>
#include <stack>

#include "Frame.hpp"
#include "GC.hpp"
#include "Instruction.hpp"
#include "Registry.hpp"

namespace svm
{
    struct Program;

    class VM
    {
    public:
        VM(size_t initialRegistrySize = 256);

        void load(const Program& program);

        void run();

        uint64_t callStackSize() const;

    private:
        void interpret(Instruction instr, Frame& frame);
        Value getRegister(Register idx) const;

        std::stack<Frame> callStack;

        Registry registry;
        const Program* program;

        GC gc;
    };
}
