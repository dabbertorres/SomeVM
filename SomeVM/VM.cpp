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
					auto dest = instr->arg1_24();
					auto src = instr->arg2_32();
					registry.at(dest) = registry.at(src);
					break;
				}

				case Instruction::Type::LoadC:
				{
					auto dest = instr->arg1_24();
					auto src = instr->arg2_32();
					registry.at(dest) = constants.at(src);
					break;
				}

				/* math ops */
				// integer
				case Instruction::Type::Add:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one + two};
					break;
				}

				case Instruction::Type::Sub:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());
					
					registry.at(instr->arg1_16()) = {one - two};
					break;
				}

				case Instruction::Type::Mult:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one * two};
					break;
				}

				case Instruction::Type::Div:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one / two};
					break;
				}

				case Instruction::Type::Mod:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one % two};
					break;
				}

				case Instruction::Type::Neg:
				{
					Int one = registry.at(instr->arg2_16());

					registry.at(instr->arg1_16()) = {-one};
					break;
				}

				// Float
				case Instruction::Type::FAdd:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one + two};
					break;
				}

				case Instruction::Type::FSub:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one - two};
					break;
				}

				case Instruction::Type::FMult:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one * two};
					break;
				}

				case Instruction::Type::FDiv:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one / two};
					break;
				}

				case Instruction::Type::FNeg:
				{
					Float one = registry.at(instr->arg2_16());

					registry.at(instr->arg1_16()) = {-one};
					break;
				}

				// misc
				case Instruction::Type::CastI:
				{
					Float one = registry.at(instr->arg2_32());

					registry.at(instr->arg1_24()) = {static_cast<Int>(one)};
					break;
				}

				case Instruction::Type::CastF:
				{
					Int one = registry.at(instr->arg2_32());

					registry.at(instr->arg1_24()) = {static_cast<Float>(one)};
					break;
				}

				/* comparison ops */
				// integer
				case Instruction::Type::Lt:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one < two};
					break;
				}

				case Instruction::Type::LtEq:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one <= two};
					break;
				}

				case Instruction::Type::Gt:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one > two};
					break;
				}

				case Instruction::Type::GtEq:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one >= two};
					break;
				}

				case Instruction::Type::Eq:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one == two};
					break;
				}

				case Instruction::Type::Neq:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one != two};
					break;
				}

				// Float
				case Instruction::Type::FLt:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one < two};
					break;
				}

				case Instruction::Type::FLtEq:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one <= two};
					break;
				}

				case Instruction::Type::FGt:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one > two};
					break;
				}

				case Instruction::Type::FGtEq:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one >= two};
					break;
				}

				case Instruction::Type::FEq:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one == two};
					break;
				}

				case Instruction::Type::FNeq:
				{
					Float one = registry.at(instr->arg2_16());
					Float two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one != two};
					break;
				}

				/* logical ops */
				case Instruction::Type::Not:
				{
					Bool one = registry.at(instr->arg2_32());

					registry.at(instr->arg1_24()) = {!one};
					break;
				}

				case Instruction::Type::And:
				{
					Bool one = registry.at(instr->arg2_16());
					Bool two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one && two};
					break;
				}

				case Instruction::Type::Or:
				{
					Bool one = registry.at(instr->arg2_16());
					Bool two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one || two};
					break;
				}

				case Instruction::Type::Xor:
				{
					Bool one = registry.at(instr->arg2_16());
					Bool two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one != two};
					break;
				}

				/* bitwise ops */
				case Instruction::Type::BAnd:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one & two};
					break;
				}

				case Instruction::Type::BOr:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one | two};
					break;
				}

				case Instruction::Type::BXor:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one ^ two};
					break;
				}

				case Instruction::Type::Bsl:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one << two};
					break;
				}

				case Instruction::Type::Bsr:
				{
					Int one = registry.at(instr->arg2_16());
					Int two = registry.at(instr->arg3_16());

					registry.at(instr->arg1_16()) = {one >> two};
					break;
				}

				/* conditional branching */
				case Instruction::Type::JmpT:
				{
					Bool b = registry.at(instr->arg1_24());
					Int idx = registry.at(instr->arg2_32());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.jump(idx);

					break;
				}

				case Instruction::Type::JmpF:
				{
					Bool b = registry.at(instr->arg1_24());
					Int idx = registry.at(instr->arg2_32());

					// if false, skip the next instruction (the jump to the "else")
					if(!b)
						frame.jump(idx);

					break;
				}

				case Instruction::Type::JmpTC:
				{
					Bool b = registry.at(instr->arg1_24());
					Int idx = constants.at(instr->arg2_32());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.jump(idx);

					break;
				}

				case Instruction::Type::JmpFC:
				{
					Bool b = registry.at(instr->arg1_24());
					Int idx = constants.at(instr->arg2_32());

					// if false, skip the next instruction (the jump to the "else")
					if(!b)
						frame.jump(idx);

					break;
				}

				case Instruction::Type::RJmpT:
				{
					Bool b = registry.at(instr->arg1_24());
					Int off = registry.at(instr->arg2_32());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.rjump(off);

					break;
				}

				case Instruction::Type::RJmpF:
				{
					Bool b = registry.at(instr->arg1_24());
					Int off = registry.at(instr->arg2_32());

					// if false, skip the next instruction (the jump to the "else")
					if(!b)
						frame.rjump(off);

					break;
				}

				case Instruction::Type::RJmpTC:
				{
					Bool b = registry.at(instr->arg1_24());
					Int off = constants.at(instr->arg2_32());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.rjump(off);

					break;
				}

				case Instruction::Type::RJmpFC:
				{
					Bool b = registry.at(instr->arg1_24());
					Int off = constants.at(instr->arg2_32());

					// if false, skip the next instruction (the jump to the "else")
					if(!b)
						frame.rjump(off);

					break;
				}

				/* branching */
				case Instruction::Type::Call:
				{
					Int nargs = registry.at(instr->arg1_16());
					Int argIdx = registry.at(instr->arg2_16());
					Int funcIdx = registry.at(instr->arg3_16());

					const Function& callee = functions[funcIdx];

					if(nargs != callee.args())
						throw std::logic_error("Invalid number of arguments!");

					callStack.emplace(callee, argIdx);
					break;
				}

				case Instruction::Type::Ret:
				{
					auto nrets = registry.at(instr->arg1_24());
					auto retIdx = registry.at(instr->arg2_32());

					callStack.pop();
					break;
				}

				case Instruction::Type::Jmp:
				{
					Int idx = registry.at(instr->arg1_56());
					frame.jump(idx);
					break;
				}

				case Instruction::Type::RJmp:
				{
					Int off = registry.at(instr->arg1_56());
					frame.rjump(off);
					break;
				}

				case Instruction::Type::JmpC:
				{
					Int idx = constants.at(instr->arg1_56());
					frame.jump(idx);
					break;
				}

				case Instruction::Type::RJmpC:
				{
					Int off = constants.at(instr->arg1_56());
					frame.rjump(off);
					break;
				}

				case Instruction::Type::Nop:
				{
					break;
				}

				case Instruction::Type::Print:
				{
					auto& val = registry.at(instr->arg1_56());

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
