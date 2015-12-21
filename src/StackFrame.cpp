#include "StackFrame.hpp"

#include <cmath>

namespace dbr
{
	namespace svm
	{
		StackFrame::StackFrame()
		:	registry(),
			nextEmpty(registry.begin())
		{}

		StackFrame::StackFrame(const Bytecode& bc)
		:	code(bc),
			currInst(code.begin()),
			nextEmpty(registry.begin())
		{}

		StackFrame::StackFrame(Registry::const_iterator begin, Registry::const_iterator end)
		:	currInst(code.begin())
		{
			initRegistry(begin, end);
		}

		StackFrame::StackFrame(const Bytecode& bc, Registry::const_iterator begin, Registry::const_iterator end)
		:	code(bc),
			currInst(code.begin())
		{
			initRegistry(begin, end);
		}

		void StackFrame::push(const Instruction& inst)
		{
			auto diff = 0;

			if(code.size() != 0)
				diff = currInst - code.begin();

			code.push_back(inst);

			currInst = code.begin() + diff;
		}

		bool StackFrame::run(const Constants& constants, const Functions& functions)
		{
			auto arg1 = currInst->arg1();
			auto arg1x = currInst->arg1x();
			auto arg2 = currInst->arg2();
			auto arg2x = currInst->arg2x();
			auto arg3 = currInst->arg3();

			switch(currInst->type())
			{
				// memory ops
				case Instruction::Type::Load:
				{
					write(constants[currInst->arg2x()], currInst->arg1());
					break;
				}

				// math ops
				case Instruction::Type::Add:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];
					
					write({one + two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Subtract:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one - two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Multiply:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one * two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Divide:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one / two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Modulus:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({std::fmod(one, two)}, currInst->arg1());
					break;
				}

				case Instruction::Type::Negative:
				{
					number one = registry[currInst->arg2()];

					write({-one}, currInst->arg1());
					break;
				}

				// comparison
				case Instruction::Type::Not:
				{
					bool one = registry[currInst->arg2()];

					write({!one}, currInst->arg1());
					break;
				}

				case Instruction::Type::Lesser:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one < two}, currInst->arg1());
					break;
				}

				case Instruction::Type::LesserOrEqual:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one <= two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Greater:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one > two}, currInst->arg1());
					break;
				}

				case Instruction::Type::GreaterOrEqual:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one >= two}, currInst->arg1());
					break;
				}

				case Instruction::Type::IsEqual:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one == two}, currInst->arg1());
					break;
				}

				case Instruction::Type::NotEqual:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one != two}, currInst->arg1());
					break;
				}

				// conditions
				case Instruction::Type::If:
				{
					bool one = registry[currInst->arg1()];

					// if true, skip the next instruction (the jump to the "else")
					if(one)
						++currInst;

					break;
				}

				// branching
				case Instruction::Type::Call:
				{
					throw std::exception("Call is not implemented");

					auto callee = functions[currInst->arg1x()];

					break;
				}

				case Instruction::Type::Return:
				{
					auto twoX = currInst->arg2x();

					if(twoX <= 0xFF)
					{
						auto one = currInst->arg1();
						
						returnPair.first = registry.begin() + one;
						returnPair.second = registry.begin() + twoX;
					}
					else
					{
						returnPair.first = registry.end();
						returnPair.second = registry.end();
					}

					currInst = code.end();

					break;
				}

				case Instruction::Type::Jump:
				{
					currInst = code.begin() + currInst->arg1x();
					break;
				}

				case Instruction::Type::Noop:
				{
					break;
				}
			}

			++currInst;

			return currInst == code.end();
		}

		StackFrame::Return StackFrame::getReturn() const
		{
			return returnPair;
		}

		void StackFrame::write(const Value& val)
		{
			if(nextEmpty == registry.end())
				nextEmpty = registry.begin();

			*nextEmpty++ = val;
		}

		void StackFrame::write(const Value& val, std::size_t idx)
		{
			registry[idx] = val;
		}

		Value StackFrame::read(std::size_t idx) const
		{
			return registry.at(idx);
		}

		void StackFrame::initRegistry(Registry::const_iterator begin, Registry::const_iterator end)
		{
			nextEmpty = std::copy(begin, end, registry.begin());
		}
	}
}