#include "VM.hpp"

namespace dbr
{
	namespace svm
	{
		VM::VM(std::istream& in, std::ostream& out, std::size_t initialRegistrySize)
		:	in(in),
			out(out),
			registry(initialRegistrySize),
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
			callStack.emplace(functions.front(), 0);

			while(!callStack.empty())
			{
				StackFrame& frame = callStack.top();

				if(!frame.complete())
					interpret();
				else
					callStack.pop();
			}
		}

		std::size_t VM::callStackSize() const
		{
			return callStack.size();
		}

		std::size_t VM::registrySize() const
		{
			return registry.size();
		}

		void VM::write(Value val)
		{
			if(nextFree != registry.end())
			{
				*nextFree++ = val;
			}
			else
			{
				registry.push_back(val);
				nextFree = registry.end();
			}
		}

		void VM::write(std::size_t idx, Value val)
		{
			registry.at(idx) = val;
		}

		Value VM::read(std::size_t idx) const
		{
			return registry.at(idx);
		}

		void VM::interpret()
		{
			auto& frame = callStack.top();
			auto instr = frame.next();

			switch(instr->type())
			{
				/* memory ops */
				case Instruction::Type::Load:
				{
					auto dest = instr->arg1();
					auto src = instr->arg2();
					registry.at(dest) = registry.at(src);
					break;
				}

				case Instruction::Type::LoadC:
				{
					auto dest = instr->arg1();
					auto src = instr->arg2x();
					registry.at(dest) = constants[src];
					break;
				}

				/* math ops */
				// integer
				case Instruction::Type::Add:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one + two};
					break;
				}

				case Instruction::Type::Sub:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one - two};
					break;
				}

				case Instruction::Type::Mult:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one * two};
					break;
				}

				case Instruction::Type::Div:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one / two};
					break;
				}

				case Instruction::Type::Mod:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one % two};
					break;
				}

				case Instruction::Type::Neg:
				{
					Int one = registry.at(instr->arg2());

					registry.at(instr->arg1()) = {-one};
					break;
				}

				// Float
				case Instruction::Type::FAdd:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one + two};
					break;
				}

				case Instruction::Type::FSub:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one - two};
					break;
				}

				case Instruction::Type::FMult:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one * two};
					break;
				}

				case Instruction::Type::FDiv:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one / two};
					break;
				}

				case Instruction::Type::FNeg:
				{
					Float one = registry.at(instr->arg2());

					registry.at(instr->arg1()) = {-one};
					break;
				}

				// misc
				case Instruction::Type::CastI:
				{
					Float one = registry.at(instr->arg2());

					registry.at(instr->arg1()) = {static_cast<Int>(one)};
					break;
				}

				case Instruction::Type::CastF:
				{
					Int one = registry.at(instr->arg2());

					registry.at(instr->arg1()) = {static_cast<Float>(one)};
					break;
				}

				/* comparison ops */
				// integer
				case Instruction::Type::Lt:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one < two};
					break;
				}

				case Instruction::Type::LtEq:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one <= two};
					break;
				}

				case Instruction::Type::Gt:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one > two};
					break;
				}

				case Instruction::Type::GtEq:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one >= two};
					break;
				}

				case Instruction::Type::Eq:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one == two};
					break;
				}

				case Instruction::Type::Neq:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one != two};
					break;
				}

				// Float
				case Instruction::Type::FLt:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one < two};
					break;
				}

				case Instruction::Type::FLtEq:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one <= two};
					break;
				}

				case Instruction::Type::FGt:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one > two};
					break;
				}

				case Instruction::Type::FGtEq:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one >= two};
					break;
				}

				case Instruction::Type::FEq:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one == two};
					break;
				}

				case Instruction::Type::FNeq:
				{
					Float one = registry.at(instr->arg2());
					Float two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one != two};
					break;
				}

				/* logical ops */
				case Instruction::Type::Not:
				{
					Bool one = registry.at(instr->arg2());

					registry.at(instr->arg1()) = {!one};
					break;
				}

				case Instruction::Type::And:
				{
					Bool one = registry.at(instr->arg2());
					Bool two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one && two};
					break;
				}

				case Instruction::Type::Or:
				{
					Bool one = registry.at(instr->arg2());
					Bool two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one || two};
					break;
				}

				case Instruction::Type::Xor:
				{
					Bool one = registry.at(instr->arg2());
					Bool two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one != two};
					break;
				}

				/* bitwise ops */
				case Instruction::Type::BAnd:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one & two};
					break;
				}

				case Instruction::Type::BOr:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one | two};
					break;
				}

				case Instruction::Type::BXor:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one ^ two};
					break;
				}

				case Instruction::Type::Bsl:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one << two};
					break;
				}

				case Instruction::Type::Bsr:
				{
					Int one = registry.at(instr->arg2());
					Int two = registry.at(instr->arg3());

					registry.at(instr->arg1()) = {one >> two};
					break;
				}

				/* conditional branching */
				case Instruction::Type::JmpT:
				{
					Bool b = registry.at(instr->arg1());
					Int idx = registry.at(instr->arg2x());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.jump(idx);

					break;
				}

				case Instruction::Type::JmpF:
				{
					Bool b = registry.at(instr->arg1());
					Int idx = registry.at(instr->arg2x());

					// if true, skip the next instruction (the jump to the "else")
					if(!b)
						frame.jump(idx);

					break;
				}
				case Instruction::Type::RJmpT:
				{
					Bool b = registry.at(instr->arg1());
					Int off = registry.at(instr->arg2x());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.rjump(off);

					break;
				}

				case Instruction::Type::RJmpF:
				{
					Bool b = registry.at(instr->arg1());
					Int off = registry.at(instr->arg2x());

					// if true, skip the next instruction (the jump to the "else")
					if(!b)
						frame.rjump(off);

					break;
				}

				/* branching */
				case Instruction::Type::Call:
				{
					Int nargs = registry.at(instr->arg1());
					Int argIdx = registry.at(instr->arg2());
					Int funcIdx = registry.at(instr->arg3());

					const Function& callee = functions[funcIdx];

					if(nargs != callee.args())
						throw std::logic_error("Invalid number of arguments!");

					callStack.emplace(callee, argIdx);
					break;
				}

				case Instruction::Type::Ret:
				{
					auto nrets = registry.at(instr->arg1());
					auto retIdx = registry.at(instr->arg2());

					callStack.pop();
					break;
				}

				case Instruction::Type::Jmp:
				{
					Int off = registry.at(instr->arg1x());
					frame.jump(off);
					break;
				}

				case Instruction::Type::RJmp:
				{
					Int off = registry.at(instr->arg1xs());
					frame.rjump(off);
					break;
				}

				case Instruction::Type::Nop:
				{
					break;
				}

				case Instruction::Type::Print:
				{
					auto& val = registry.at(instr->arg1());

					switch(val.type())
					{
						case Value::Type::Nil:
							out << "Nil";
							break;

						case Value::Type::Bool:
							out << std::boolalpha << static_cast<Bool>(val);
							break;

						case Value::Type::Int:
							out << static_cast<Int>(val);
							break;

						case Value::Type::Float:
							out << static_cast<Float>(val);
							break;

						case Value::Type::String:
							out << static_cast<String>(val);
							break;
					}

					out << std::endl;
					break;
				}
			}
		}
	}
}
