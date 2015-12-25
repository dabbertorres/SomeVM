#include "StackFrame.hpp"

#include <cmath>

namespace
{
	struct Float
	{
		Float()
		:	valid(false)
		{}

		float value;
		bool valid;
	};

	struct Int
	{
		Int()
		: valid(false)
		{}

		int value;
		bool valid;
	};
}

namespace dbr
{
	namespace svm
	{
		StackFrame::StackFrame()
		:	registry()
		{}

		StackFrame::StackFrame(const Bytecode& bc)
		:	code(bc),
			currInst(code.begin()),
			registry()
		{}

		StackFrame::StackFrame(Registry::const_iterator begin, Registry::const_iterator end)
		:	currInst(code.begin())
		{
			std::copy(begin, end, registry.begin());
		}

		StackFrame::StackFrame(const Bytecode& bc, Registry::const_iterator begin, Registry::const_iterator end)
		:	code(bc),
			currInst(code.begin())
		{
			std::copy(begin, end, registry.begin());
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

			std::copy(other.registry.begin(), other.registry.end(), registry.begin());

			return *this;
		}

		StackFrame& StackFrame::operator=(StackFrame&& other)
		{
			code = std::move(other.code);
			currInst = std::move(other.currInst);
			registry = std::move(other.registry);

			other.currInst = other.code.end();

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
			switch(currInst->type())
			{
				/* memory ops */
				case Instruction::Type::Load:
				{
					write(currInst->arg1(), registry[currInst->arg2()]);
					break;
				}
				
				case Instruction::Type::LoadC:
				{
					write(currInst->arg1(), constants[currInst->arg2x()]);
					break;
				}

				/* math ops */
				// integer
				case Instruction::Type::Add:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one + two});
					break;
				}

				case Instruction::Type::Sub:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one - two});
					break;
				}

				case Instruction::Type::Mult:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one * two});
					break;
				}

				case Instruction::Type::Div:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one / two});
					break;
				}

				case Instruction::Type::Mod:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one % two});
					break;
				}

				case Instruction::Type::Neg:
				{
					int one = registry[currInst->arg2()];

					write(currInst->arg1(), {-one});
					break;
				}

				// float
				case Instruction::Type::FAdd:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one + two});
					break;
				}

				case Instruction::Type::FSub:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one - two});
					break;
				}

				case Instruction::Type::FMult:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one * two});
					break;
				}

				case Instruction::Type::FDiv:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one / two});
					break;
				}

				case Instruction::Type::FNeg:
				{
					float one = registry[currInst->arg2()];

					write(currInst->arg1(), {-one});
					break;
				}

				// misc
				case Instruction::Type::CastI:
				{
					float one = registry[currInst->arg2()];

					write(currInst->arg1(), {static_cast<int>(one)});
					break;
				}

				case Instruction::Type::CastF:
				{
					int one = registry[currInst->arg2()];

					write(currInst->arg1(), {static_cast<float>(one)});
					break;
				}

				/* comparison ops */
				// integer
				case Instruction::Type::Lt:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one < two});
					break;
				}

				case Instruction::Type::LtEq:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one <= two});
					break;
				}

				case Instruction::Type::Gt:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one > two});
					break;
				}

				case Instruction::Type::GtEq:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one >= two});
					break;
				}

				case Instruction::Type::Eq:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one == two});
					break;
				}

				case Instruction::Type::Neq:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one != two});
					break;
				}

				// float
				case Instruction::Type::FLt:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one < two});
					break;
				}

				case Instruction::Type::FLtEq:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one <= two});
					break;
				}

				case Instruction::Type::FGt:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one > two});
					break;
				}

				case Instruction::Type::FGtEq:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one >= two});
					break;
				}

				case Instruction::Type::FEq:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one == two});
					break;
				}

				case Instruction::Type::FNeq:
				{
					float one = registry[currInst->arg2()];
					float two = registry[currInst->arg3()];

					write(currInst->arg1(), {one != two});
					break;
				}

				/* logical ops */
				case Instruction::Type::Not:
				{
					bool one = registry[currInst->arg2()];

					write(currInst->arg1(), {!one});
					break;
				}

				case Instruction::Type::And:
				{
					bool one = registry[currInst->arg2()];
					bool two = registry[currInst->arg3()];

					write(currInst->arg1(), {one && two});
					break;
				}

				case Instruction::Type::Or:
				{
					bool one = registry[currInst->arg2()];
					bool two = registry[currInst->arg3()];

					write(currInst->arg1(), {one || two});
					break;
				}

				case Instruction::Type::Xor:
				{
					bool one = registry[currInst->arg2()];
					bool two = registry[currInst->arg3()];

					write(currInst->arg1(), {one != two});
					break;
				}

				/* bitwise ops */
				case Instruction::Type::BAnd:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one & two});
					break;
				}

				case Instruction::Type::BOr:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one | two});
					break;
				}

				case Instruction::Type::BXor:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one ^ two});
					break;
				}

				case Instruction::Type::Bsl:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one << two});
					break;
				}

				case Instruction::Type::Bsr:
				{
					int one = registry[currInst->arg2()];
					int two = registry[currInst->arg3()];

					write(currInst->arg1(), {one >> two});
					break;
				}

				/* conditions */
				case Instruction::Type::If:
				{
					bool one = registry[currInst->arg1()];

					// if true, skip the next instruction (the jump to the "else")
					if(one)
						++currInst;

					break;
				}

				/* branching */
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

						case Value::Type::Int:
							out << static_cast<int>(val);
							break;

						case Value::Type::Float:
							out << static_cast<float>(val);
							break;

						case Value::Type::String:
							out << static_cast<string>(val);
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

		void StackFrame::write(std::size_t idx, const Value& val)
		{
			registry[idx] = val;
		}

		Value StackFrame::read(std::size_t idx) const
		{
			return registry.at(idx);
		}
	}
}