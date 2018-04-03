#include "VM.hpp"

#include <chrono>
#include <cmath>
#include <iterator>

#include "Program.hpp"
#include "SysCall.hpp"
#include "util.hpp"

using namespace std::chrono_literals;

namespace svm
{
    VM::VM(size_t initialRegistrySize)
        : registry{ initialRegistrySize },
        program{ nullptr },
        gc(registry, 500ms)
    {}

    void VM::load(const Program& prog)
    {
        while (!callStack.empty())
            callStack.pop();

        this->program = &prog;
    }

    void VM::run()
    {
        // TODO make REPL friendly
        callStack.emplace(program->functions.front(), 0_r, 0_u16);

        while (!callStack.empty())
        {
            Frame& frame = callStack.top();

            if (frame.next() != frame.end())
                interpret(*frame.next(), frame);
            else
                callStack.pop();
        }
    }

    std::uint64_t VM::callStackSize() const
    {
        return callStack.size();
    }

    void VM::interpret(Instruction instr, Frame& frame)
    {
        // memory ops
        Register dest;

        // operations
        Value lhs;
        Value rhs;
        Value val;

        // jumps
        Value condition;
        Value idx;

        // function calls
        uint16_t nargs;
        Register argIdx;
        uint16_t funcIdx;

        switch (instr.type())
        {
        /* memory ops */
        case Instruction::Code::Load:
            dest = instr.arg1();
            val = getRegister(instr.arg2());
            registry.set(dest, val);
            break;

        /* integer math */
        case Instruction::Code::Add:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) + std::get<Int>(rhs) });
            break;

        case Instruction::Code::Sub:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) - std::get<Int>(rhs) });
            break;

        case Instruction::Code::Mult:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) * std::get<Int>(rhs) });
            break;

        case Instruction::Code::Div:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) / std::get<Int>(rhs) });
            break;

        case Instruction::Code::Mod:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) % std::get<Int>(rhs) });
            break;

        case Instruction::Code::Neg:
            lhs = getRegister(instr.arg2());
            registry.set(instr.arg1(), Value{ -std::get<Int>(lhs) });
            break;

        /* floating-point math */
        case Instruction::Code::Addf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) + std::get<Float>(rhs) });
            break;

        case Instruction::Code::Subf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) - std::get<Float>(rhs) });
            break;

        case Instruction::Code::Multf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) * std::get<Float>(rhs) });
            break;

        case Instruction::Code::Divf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) / std::get<Float>(rhs) });
            break;

        case Instruction::Code::Modf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::fmod(std::get<Float>(lhs), std::get<Float>(rhs)) });
            break;

        case Instruction::Code::Negf:
            lhs = getRegister(instr.arg2());
            registry.set(instr.arg1(), Value{ -std::get<Float>(lhs) });
            break;

        /* integer comparison */
        case Instruction::Code::Lt:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());

            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) < std::get<Int>(rhs) });
            break;

        case Instruction::Code::LtEq:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) <= std::get<Int>(rhs) });
            break;

        case Instruction::Code::Gt:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) > std::get<Int>(rhs) });
            break;

        case Instruction::Code::GtEq:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) >= std::get<Int>(rhs) });
            break;

        case Instruction::Code::Eq:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) == std::get<Int>(rhs) });
            break;

        case Instruction::Code::Neq:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Int>(lhs) != std::get<Int>(rhs) });
            break;

        /* floating-point comparison */
        case Instruction::Code::Ltf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());

            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) < std::get<Float>(rhs) });
            break;

        case Instruction::Code::LtEqf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) <= std::get<Float>(rhs) });
            break;

        case Instruction::Code::Gtf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) > std::get<Float>(rhs) });
            break;

        case Instruction::Code::GtEqf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) >= std::get<Float>(rhs) });
            break;

        case Instruction::Code::Eqf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) == std::get<Float>(rhs) });
            break;

        case Instruction::Code::Neqf:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Float>(lhs) != std::get<Float>(rhs) });
            break;

        /* boolean logic */
        case Instruction::Code::Not:
            lhs = getRegister(instr.arg2());
            registry.set(instr.arg1(), Value{ !std::get<Bool>(lhs) });
            break;

        case Instruction::Code::And:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Bool>(lhs) && std::get<Bool>(rhs) });
            break;

        case Instruction::Code::Or:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Bool>(lhs) || std::get<Bool>(rhs) });
            break;

        case Instruction::Code::Xor:
            lhs = getRegister(instr.arg2());
            rhs = getRegister(instr.arg3());
            registry.set(instr.arg1(), Value{ std::get<Bool>(lhs) != std::get<Bool>(rhs) && (std::get<Bool>(lhs) || std::get<Bool>(rhs)) });
            break;

        /* conditional branching */
        case Instruction::Code::JmpT:
            condition = getRegister(instr.arg1());
            idx = program->constants.at(instr.arg2());

            // if true, skip the next instruction (the jump to the "else")
            if (std::get<Bool>(condition))
                frame.jump(std::get<Int>(idx));

            break;

        case Instruction::Code::JmpF:
            condition = registry.get(instr.arg1());
            idx = program->constants.at(instr.arg2());

            // if false, skip the next instruction (the jump to the "else")
            if (!std::get<Bool>(condition))
                frame.jump(std::get<Int>(idx));

            break;

        /* branching */
        case Instruction::Code::Call:
            nargs = instr.arg1();
            argIdx = instr.arg2();
            funcIdx = instr.arg3();

            callStack.emplace(program->functions[funcIdx], argIdx, nargs);
            break;

        case Instruction::Code::Ret:
            // TODO
            //nrets = getInteger(registry.at(instr.arg1_24()));
            //retIdx = getInteger(registry.at(instr.arg2_32()));

            callStack.pop();
            break;

        case Instruction::Code::Jmp:
            idx = program->constants.at(instr.arg1());
            frame.jump(std::get<Int>(idx));
            break;

        case Instruction::Code::SysCall:
            switch (static_cast<SysCall>(instr.arg1()))
            {
            case SysCall::PrintBool:
                val = registry.get(instr.arg2());
                std::printf("%s", std::get<Bool>(val) ? "true" : "false");
                break;

            case SysCall::PrintInt:
                val = registry.get(instr.arg2());
                std::printf("%lld", std::get<Int>(val));
                break;

            case SysCall::PrintFloat:
                val = registry.get(instr.arg2());
                std::printf("%f", std::get<Float>(val));
                break;

            case SysCall::Print:
                // an object that wants to be printed is going to have it's string() method
                // called before this - so the argIdx is pointing to a string (the result)
                val = registry.get(instr.arg2());
                std::printf("%.*s", static_cast<int>(std::get<String>(val).length()), std::get<String>(val).data());
                break;

            default:
                // TODO just ignore unknown syscall ids for now
                break;
            }
            break;

        case Instruction::Code::Nop:
            break;
        }
    }

    Value VM::getRegister(Register idx) const
    {
        if (isConstant(idx))
            return program->constants.at(idx);
        else
            return registry.get(idx);
    }
}
