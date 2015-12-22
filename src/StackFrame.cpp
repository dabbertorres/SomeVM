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
			registry(),
			nextEmpty(registry.begin())
		{}

		StackFrame::StackFrame(Registry::const_iterator begin, Registry::const_iterator end)
		:	currInst(code.begin())
		{
			nextEmpty = std::copy(begin, end, registry.begin());
		}

		StackFrame::StackFrame(const Bytecode& bc, Registry::const_iterator begin, Registry::const_iterator end)
		:	code(bc),
			currInst(code.begin())
		{
			nextEmpty = std::copy(begin, end, registry.begin());
		}

		StackFrame::StackFrame(const StackFrame& other)
		{
			*this = other;
		}

		StackFrame::StackFrame(StackFrame&& other)
		{
			*this = other;
		}

		StackFrame& StackFrame::operator=(const StackFrame& other)
		{
			code.resize(other.code.size());
			std::copy(other.code.begin(), other.code.end(), code.begin());

			currInst = code.begin() + (other.currInst - other.code.begin());

			nextEmpty = std::copy(other.registry.begin(), other.registry.end(), registry.begin());

			return *this;
		}

		StackFrame& StackFrame::operator=(StackFrame&& other)
		{
			code = std::move(other.code);
			currInst = std::move(other.currInst);
			registry = std::move(other.registry);
			nextEmpty = std::move(other.nextEmpty);

			other.currInst = other.code.end();
			other.nextEmpty = other.registry.end();

			return *this;
		}

		void StackFrame::push(const Instruction& inst)
		{
			auto diff = 0;

			if(code.size() != 0)
				diff = currInst - code.begin();

			code.push_back(inst);

			currInst = code.begin() + diff;
		}

		bool StackFrame::run(Constants& constants, const Functions& functions, std::istream& in, std::ostream& out)
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

				case Instruction::Type::Sub:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one - two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Mult:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one * two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Div:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one / two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Mod:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({std::fmod(one, two)}, currInst->arg1());
					break;
				}

				case Instruction::Type::Neg:
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

				case Instruction::Type::Less:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one < two}, currInst->arg1());
					break;
				}

				case Instruction::Type::LessEq:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one <= two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Gret:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one > two}, currInst->arg1());
					break;
				}

				case Instruction::Type::GretEq:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one >= two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Eq:
				{
					number one = registry[currInst->arg2()];
					number two = registry[currInst->arg3()];

					write({one == two}, currInst->arg1());
					break;
				}

				case Instruction::Type::Neq:
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

				case Instruction::Type::Ret:
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

				case Instruction::Type::Print:
				{
					auto reg = currInst->arg1();

					const Value& val = registry.at(reg);

					switch(val.type())
					{
						case Value::Type::Nil:
							out << "nil";
							break;

						case Value::Type::Bool:
							out << std::boolalpha << static_cast<bool>(val);
							break;

						case Value::Type::Number:
							out << static_cast<number>(val);
							break;

						case Value::Type::String:
							out << '"' << static_cast<string>(val) << '"';
							break;
					}

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
	}
}