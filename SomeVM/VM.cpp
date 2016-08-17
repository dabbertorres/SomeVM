#include "VM.hpp"

#ifndef _MSC_VER
#	ifdef __GNUC__
#		if defined(__x86_64__) || defined(__i386__)
#			include <x86intrin.h>
#		elif defined(__ARM_NEON__)
#			include <arm_neon.h>
#		elif defined(__IWMMXT__)
#			include <mmintrin.h>
#		endif
#	else
#		warning "SIMD instructions are not supported on this target platform"
#		define SOME_VM_NO_SIMD
#	endif
#else
#	include <intrin.h>	// Visual C++ handles getting the platform-specific header(s) for us
#endif

namespace dbr
{
	namespace svm
	{
		VM::VM(std::istream& inStream, std::ostream& outStream, std::size_t initialRegistrySize)
		:	inStream(inStream),
			outStream(outStream),
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
			callStack.emplace(functions.front(), 0, 0);

			while(!callStack.empty())
			{
				StackFrame& frame = callStack.top();

				if(!frame.complete())
					interpret(*frame.next(), frame);
				else
					callStack.pop();
			}
		}

		void VM::debug(const std::vector<Breakpoint>& breakpoints)
		{
			callStack.emplace(functions.front(), 0, 0);

			std::size_t nextBreakpoint = 0;

			while(!callStack.empty())
			{
				StackFrame& frame = callStack.top();
				
				if(!frame.complete())
				{
					auto next = frame.next();

					if(nextBreakpoint < breakpoints.size())
					{
						const Breakpoint& bp = breakpoints[nextBreakpoint++];

						if(bp.functionIndex == frame.functionIndex)
						{
							std::size_t instIdx = std::distance(frame.begin(), next);

							if(bp.instructionIndex == instIdx)
							{
								// May want a different design.
								// Debugger class, which takes a VM as an argument
								// and runs from there.

								std::string input;
								do
								{
									outStream << "> ";

									std::getline(inStream, input);
								}
								while(input != "c");
							}
						}
					}

					interpret(*next, frame);
				}
				else
				{
					callStack.pop();
				}
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

		void VM::print(const Value& val) const
		{
			switch(val.type())
			{
				case Value::Type::Nil:
					outStream << "Nil";
					break;

				case Value::Type::Bool:
					outStream << std::boolalpha << static_cast<Bool>(val);
					break;

				case Value::Type::Int:
					outStream << static_cast<Int>(val);
					break;

				case Value::Type::Float:
					outStream << static_cast<Float>(val);
					break;

				case Value::Type::String:
					outStream << val.operator String();
					break;
			}

			outStream << std::endl;
		}

		void VM::interpret(Instruction instr, StackFrame& frame)
		{
			switch(instr.type())
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
				// integer
				case Instruction::Type::Add:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one + two};
					break;
				}

				case Instruction::Type::Sub:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one - two};
					break;
				}

				case Instruction::Type::Mult:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one * two};
					break;
				}

				case Instruction::Type::Div:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one / two};
					break;
				}

				case Instruction::Type::Mod:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one % two};
					break;
				}

				case Instruction::Type::Neg:
				{
					Int one = registry.at(instr.arg2_16());

					registry.at(instr.arg1_16()) = {-one};
					break;
				}

				// Float
				case Instruction::Type::FAdd:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one + two};
					break;
				}

				case Instruction::Type::FSub:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one - two};
					break;
				}

				case Instruction::Type::FMult:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one * two};
					break;
				}

				case Instruction::Type::FDiv:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one / two};
					break;
				}

				case Instruction::Type::FNeg:
				{
					Float one = registry.at(instr.arg2_16());

					registry.at(instr.arg1_16()) = {-one};
					break;
				}

#ifndef SOME_VM_NO_SIMD
				// might be only Visual C++ compatible right now.
				// Should add "using TYPE = COMPILER_TYPE;" in macros in 
				// another header file so a common type can be used

				case Instruction::Type::simdAdd:
				{
					__m256d lhs;
					__m256d rhs;

					auto leftIt = registry.begin() + instr.arg1_16();
					auto rightIt = registry.begin() + instr.arg2_16();

					std::copy(leftIt, leftIt + 4, lhs.m256d_f64);
					std::copy(rightIt, rightIt + 4, rhs.m256d_f64);

					__m256d out = _mm256_add_pd(lhs, rhs);

					std::copy(out.m256d_f64, out.m256d_f64 + 4, registry.begin() + instr.arg3_16());
					break;
				}

				case Instruction::Type::simdSub:
				{
					__m256d lhs;
					__m256d rhs;

					auto leftIt = registry.begin() + instr.arg1_16();
					auto rightIt = registry.begin() + instr.arg2_16();

					std::copy(leftIt, leftIt + 4, lhs.m256d_f64);
					std::copy(rightIt, rightIt + 4, rhs.m256d_f64);

					__m256d out = _mm256_sub_pd(lhs, rhs);

					std::copy(out.m256d_f64, out.m256d_f64 + 4, registry.begin() + instr.arg3_16());
					break;
				}

				case Instruction::Type::simdMult:
				{
					__m256d lhs;
					__m256d rhs;

					auto leftIt = registry.begin() + instr.arg1_16();
					auto rightIt = registry.begin() + instr.arg2_16();

					std::copy(leftIt, leftIt + 4, lhs.m256d_f64);
					std::copy(rightIt, rightIt + 4, rhs.m256d_f64);

					__m256d out = _mm256_mul_pd(lhs, rhs);

					std::copy(out.m256d_f64, out.m256d_f64 + 4, registry.begin() + instr.arg3_16());
					break;
				}

				case Instruction::Type::simdDiv:
				{
					__m256d lhs;
					__m256d rhs;

					auto leftIt = registry.begin() + instr.arg1_16();
					auto rightIt = registry.begin() + instr.arg2_16();

					std::copy(leftIt, leftIt + 4, lhs.m256d_f64);
					std::copy(rightIt, rightIt + 4, rhs.m256d_f64);

					__m256d out = _mm256_div_pd(lhs, rhs);

					std::copy(out.m256d_f64, out.m256d_f64 + 4, registry.begin() + instr.arg3_16());
					break;
				}
#endif

				// misc
				case Instruction::Type::CastI:
				{
					Float one = registry.at(instr.arg2_32());

					registry.at(instr.arg1_24()) = {static_cast<Int>(one)};
					break;
				}

				case Instruction::Type::CastF:
				{
					Int one = registry.at(instr.arg2_32());

					registry.at(instr.arg1_24()) = {static_cast<Float>(one)};
					break;
				}

				/* comparison ops */
				// integer
				case Instruction::Type::Lt:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one < two};
					break;
				}

				case Instruction::Type::LtEq:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one <= two};
					break;
				}

				case Instruction::Type::Gt:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one > two};
					break;
				}

				case Instruction::Type::GtEq:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one >= two};
					break;
				}

				case Instruction::Type::Eq:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one == two};
					break;
				}

				case Instruction::Type::Neq:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one != two};
					break;
				}

				// Float
				case Instruction::Type::FLt:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one < two};
					break;
				}

				case Instruction::Type::FLtEq:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one <= two};
					break;
				}

				case Instruction::Type::FGt:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one > two};
					break;
				}

				case Instruction::Type::FGtEq:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one >= two};
					break;
				}

				case Instruction::Type::FEq:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one == two};
					break;
				}

				case Instruction::Type::FNeq:
				{
					Float one = registry.at(instr.arg2_16());
					Float two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one != two};
					break;
				}

				/* logical ops */
				case Instruction::Type::Not:
				{
					Bool one = registry.at(instr.arg2_32());

					registry.at(instr.arg1_24()) = {!one};
					break;
				}

				case Instruction::Type::And:
				{
					Bool one = registry.at(instr.arg2_16());
					Bool two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one && two};
					break;
				}

				case Instruction::Type::Or:
				{
					Bool one = registry.at(instr.arg2_16());
					Bool two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one || two};
					break;
				}

				case Instruction::Type::Xor:
				{
					Bool one = registry.at(instr.arg2_16());
					Bool two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one != two};
					break;
				}

				/* bitwise ops */
				case Instruction::Type::BNot:
				{
					Int value = registry.at(instr.arg2_32());

					registry.at(instr.arg1_24()) = ~value;
					break;
				}

				case Instruction::Type::BAnd:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one & two};
					break;
				}

				case Instruction::Type::BOr:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one | two};
					break;
				}

				case Instruction::Type::BXor:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one ^ two};
					break;
				}

				case Instruction::Type::Bsl:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one << two};
					break;
				}

				case Instruction::Type::Bsr:
				{
					Int one = registry.at(instr.arg2_16());
					Int two = registry.at(instr.arg3_16());

					registry.at(instr.arg1_16()) = {one >> two};
					break;
				}

				/* conditional branching */
				case Instruction::Type::JmpT:
				{
					Bool b = registry.at(instr.arg1_24());
					Int idx = registry.at(instr.arg2_32());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.jump(idx);

					break;
				}

				case Instruction::Type::JmpF:
				{
					Bool b = registry.at(instr.arg1_24());
					Int idx = registry.at(instr.arg2_32());

					// if false, skip the next instruction (the jump to the "else")
					if(!b)
						frame.jump(idx);

					break;
				}

				case Instruction::Type::JmpTC:
				{
					Bool b = registry.at(instr.arg1_24());
					Int idx = constants.at(instr.arg2_32());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.jump(idx);

					break;
				}

				case Instruction::Type::JmpFC:
				{
					Bool b = registry.at(instr.arg1_24());
					Int idx = constants.at(instr.arg2_32());

					// if false, skip the next instruction (the jump to the "else")
					if(!b)
						frame.jump(idx);

					break;
				}

				case Instruction::Type::RJmpT:
				{
					Bool b = registry.at(instr.arg1_24());
					Int off = registry.at(instr.arg2_32());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.rjump(off);

					break;
				}

				case Instruction::Type::RJmpF:
				{
					Bool b = registry.at(instr.arg1_24());
					Int off = registry.at(instr.arg2_32());

					// if false, skip the next instruction (the jump to the "else")
					if(!b)
						frame.rjump(off);

					break;
				}

				case Instruction::Type::RJmpTC:
				{
					Bool b = registry.at(instr.arg1_24());
					Int off = constants.at(instr.arg2_32());

					// if true, skip the next instruction (the jump to the "else")
					if(b)
						frame.rjump(off);

					break;
				}

				case Instruction::Type::RJmpFC:
				{
					Bool b = registry.at(instr.arg1_24());
					Int off = constants.at(instr.arg2_32());

					// if false, skip the next instruction (the jump to the "else")
					if(!b)
						frame.rjump(off);

					break;
				}

				/* branching */
				case Instruction::Type::Call:
				{
					Int nargs = registry.at(instr.arg1_16());
					Int argIdx = registry.at(instr.arg2_16());
					Int funcIdx = registry.at(instr.arg3_16());

					const Function& callee = functions[funcIdx];

					if(nargs != callee.args())
						throw std::logic_error("Invalid number of arguments!");

					callStack.emplace(callee, funcIdx, argIdx);
					break;
				}

				case Instruction::Type::Ret:
				{
					auto nrets = registry.at(instr.arg1_24());
					auto retIdx = registry.at(instr.arg2_32());

					callStack.pop();
					break;
				}

				case Instruction::Type::Jmp:
				{
					Int idx = registry.at(instr.arg1_56());
					frame.jump(idx);
					break;
				}

				case Instruction::Type::RJmp:
				{
					Int off = registry.at(instr.arg1_56());
					frame.rjump(off);
					break;
				}

				case Instruction::Type::JmpC:
				{
					Int idx = constants.at(instr.arg1_56());
					frame.jump(idx);
					break;
				}

				case Instruction::Type::RJmpC:
				{
					Int off = constants.at(instr.arg1_56());
					frame.rjump(off);
					break;
				}

				case Instruction::Type::Nop:
				{
					break;
				}

				case Instruction::Type::Print:
				{
					const auto& val = registry.at(instr.arg1_56());
					print(val);
					break;
				}
			}
		}
	}
}
