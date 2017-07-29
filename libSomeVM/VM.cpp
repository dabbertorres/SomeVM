#include "VM.hpp"

#include <iterator>

#include "Program.hpp"
#include "SysCall.hpp"

namespace
{
	using namespace svm;

	std::int64_t getInteger(Float f)
	{
		constexpr auto VALUE_MASK = 0x000fffffffffffffu;

		auto& ir = reinterpret_cast<std::int64_t&>(f);

		bool sign = (ir & (1ull << 63)) != 0;
		// assume exponent is +1

		return (ir & VALUE_MASK) * (sign ? -1 : 1);
	}
}

namespace svm
{
	VM::VM(std::uint64_t initialRegistrySize)
		: registry(initialRegistrySize),
		nextFree(registry.begin())
	{}

	void VM::load(const Program& program)
	{
		// make sure we only need to do 1 allocation while inserting
		constants.reserve(constants.size() + program.constants.size());
		std::copy(program.constants.begin(), program.constants.end(), std::back_inserter(constants));

		// make sure we only need to do 1 allocation while inserting
		functions.reserve(functions.size() + program.functions.size());
		std::copy(program.functions.begin(), program.functions.end(), std::back_inserter(functions));
	}

	void VM::run()
	{
		callStack.emplace(functions.front(), 0, 0);

		while (!callStack.empty())
		{
			StackFrame& frame = callStack.top();

			if (!frame.complete())
				interpret(*frame.next(), frame);
			else
				callStack.pop();
		}
	}

	std::uint64_t VM::callStackSize() const
	{
		return callStack.size();
	}

	std::uint64_t VM::registrySize() const
	{
		return registry.size();
	}

	void VM::write(Value val)
	{
		if (nextFree != registry.end())
		{
			*nextFree++ = val;
		}
		else
		{
			registry.push_back(val);
			nextFree = registry.end();
		}
	}

	void VM::write(std::uint64_t idx, Value val)
	{
		registry.at(idx) = val;
	}

	Value VM::read(std::uint64_t idx) const
	{
		return registry.at(idx);
	}

	void VM::interpret(Instruction instr, StackFrame& frame)
	{
		switch (instr.type())
		{
			/* memory ops */
		case Instruction::Type::Load:
		{
			auto dest = instr.arg1_24();
			auto src = instr.arg2_32();
			registry.at(dest) = registry.at(src);
			break;
		}

		case Instruction::Type::LoadC:
		{
			auto dest = instr.arg1_24();
			auto src = instr.arg2_32();
			registry.at(dest) = constants.at(src);
			break;
		}

		/* math ops */
		case Instruction::Type::Add:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one + two };
			break;
		}

		case Instruction::Type::Sub:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one - two };
			break;
		}

		case Instruction::Type::Mult:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one * two };
			break;
		}

		case Instruction::Type::Div:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one / two };
			break;
		}

		case Instruction::Type::Mod:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = std::fmod(one, two);
			break;
		}

		case Instruction::Type::Neg:
		{
			Float one = registry.at(instr.arg2_16());

			registry.at(instr.arg1_16()) = { -one };
			break;
		}

		/* comparison ops */
		case Instruction::Type::Lt:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one < two };
			break;
		}

		case Instruction::Type::LtEq:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one <= two };
			break;
		}

		case Instruction::Type::Gt:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one > two };
			break;
		}

		case Instruction::Type::GtEq:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one >= two };
			break;
		}

		case Instruction::Type::Eq:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one == two };
			break;
		}

		case Instruction::Type::Neq:
		{
			Float one = registry.at(instr.arg2_16());
			Float two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one != two };
			break;
		}

		/* logical ops */
		case Instruction::Type::Not:
		{
			Bool one = registry.at(instr.arg2_32());

			registry.at(instr.arg1_24()) = { !one };
			break;
		}

		case Instruction::Type::And:
		{
			Bool one = registry.at(instr.arg2_16());
			Bool two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one && two };
			break;
		}

		case Instruction::Type::Or:
		{
			Bool one = registry.at(instr.arg2_16());
			Bool two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one || two };
			break;
		}

		case Instruction::Type::Xor:
		{
			Bool one = registry.at(instr.arg2_16());
			Bool two = registry.at(instr.arg3_16());

			registry.at(instr.arg1_16()) = { one != two };
			break;
		}

		/* conditional branching */
		case Instruction::Type::JmpT:
		{
			Bool b = registry.at(instr.arg1_24());
			auto idx = getInteger(constants.at(instr.arg2_32()));

			// if true, skip the next instruction (the jump to the "else")
			if (b)
				frame.jump(idx);

			break;
		}

		case Instruction::Type::JmpF:
		{
			Bool b = registry.at(instr.arg1_24());
			auto idx = getInteger(constants.at(instr.arg2_32()));

			// if false, skip the next instruction (the jump to the "else")
			if (!b)
				frame.jump(idx);

			break;
		}

		case Instruction::Type::JmpTC:
		{
			Bool b = registry.at(instr.arg1_24());
			auto idx = getInteger(constants.at(instr.arg2_32()));

			// if true, skip the next instruction (the jump to the "else")
			if (b)
				frame.jump(idx);

			break;
		}

		case Instruction::Type::JmpFC:
		{
			Bool b = registry.at(instr.arg1_24());
			auto idx = getInteger(constants.at(instr.arg2_32()));

			// if false, skip the next instruction (the jump to the "else")
			if (!b)
				frame.jump(idx);

			break;
		}

		case Instruction::Type::RJmpT:
		{
			Bool b = registry.at(instr.arg1_24());
			auto off = getInteger(constants.at(instr.arg2_32()));

			// if true, skip the next instruction (the jump to the "else")
			if (b)
				frame.rjump(off);

			break;
		}

		case Instruction::Type::RJmpF:
		{
			Bool b = registry.at(instr.arg1_24());
			auto off = getInteger(constants.at(instr.arg2_32()));

			// if false, skip the next instruction (the jump to the "else")
			if (!b)
				frame.rjump(off);

			break;
		}

		case Instruction::Type::RJmpTC:
		{
			Bool b = registry.at(instr.arg1_24());
			auto off = getInteger(constants.at(instr.arg2_32()));

			// if true, skip the next instruction (the jump to the "else")
			if (b)
				frame.rjump(off);

			break;
		}

		case Instruction::Type::RJmpFC:
		{
			Bool b = registry.at(instr.arg1_24());
			auto off = getInteger(constants.at(instr.arg2_32()));

			// if false, skip the next instruction (the jump to the "else")
			if (!b)
				frame.rjump(off);

			break;
		}

		/* branching */
		case Instruction::Type::Call:
		{
			auto nargs = getInteger(registry.at(instr.arg1_16()));
			auto argIdx = getInteger(registry.at(instr.arg2_16()));
			auto funcIdx = getInteger(registry.at(instr.arg3_16()));

			const Function& callee = functions[funcIdx];

			if (nargs != callee.args())
				throw std::logic_error("Invalid number of arguments!");

			callStack.emplace(callee, funcIdx, argIdx);
			break;
		}

		case Instruction::Type::Ret:
		{
			// TODO: make work like it should
//					auto nrets = getInteger(registry.at(instr.arg1_24()));
//					auto retIdx = getInteger(registry.at(instr.arg2_32()));

			callStack.pop();
			break;
		}

		case Instruction::Type::Jmp:
		{
			auto idx = getInteger(registry.at(instr.arg1_56()));
			frame.jump(idx);
			break;
		}

		case Instruction::Type::RJmp:
		{
			auto off = getInteger(registry.at(instr.arg1_56()));
			frame.rjump(off);
			break;
		}

		case Instruction::Type::JmpC:
		{
			auto idx = getInteger(constants.at(instr.arg1_56()));
			frame.jump(idx);
			break;
		}

		case Instruction::Type::RJmpC:
		{
			auto off = getInteger(constants.at(instr.arg1_56()));
			frame.rjump(off);
			break;
		}

		case Instruction::Type::SysCall:
		{
			auto nargs = getInteger(registry.at(instr.arg1_16()));
			auto argIdx = getInteger(registry.at(instr.arg2_16()));
			auto funcIdx = static_cast<SysCall>(getInteger(registry.at(instr.arg3_16())));

			switch (funcIdx)
			{
			case SysCall::Print:
				for (; argIdx < argIdx + nargs; ++argIdx)
					std::printf(registry.at(argIdx), registry.at(++argIdx));
				break;

			default:
				// ignore unknown syscall ids for now
				break;
			}

			break;
		}

		case Instruction::Type::Nop:
		{
			break;
		}
		}
	}
}
