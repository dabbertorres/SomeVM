#include "VM.hpp"

#include "Instruction.hpp"
#include <iostream>

namespace lng
{
	VM::VM(unsigned int ss)
		:	MAX_STACK_SIZE(ss)
	{}

	VM::~VM()
	{}

	void VM::run(const Bytecode& bytecode)
	{
		previous = bytecode.end();
		for(auto it = bytecode.begin(); it != bytecode.end();)
		{
			switch(static_cast<Instruction>(*it))
			{
				case Instruction::PushLiteral:
					switch(static_cast<ValueType>(*(it + 1)))
					{
						case ValueType::None:
							stack.push(new Value<nil>(nullptr));
							break;

						case ValueType::Bool:
							stack.push(new Value<bool>(static_cast<bool>(*(it + 2))));
							break;

						case ValueType::Number:
							stack.push(new Value<double>(static_cast<double>(*(it + 2))));
							break;

						default:
							std::cerr << "Invalid Value in bytecode. OP: " << it - bytecode.begin() << '\n';
							throw std::runtime_error("");
							break;
					}
					
					it += 3;
					break;

				case Instruction::PushVariable:
				{
					unsigned int var = static_cast<unsigned int>(*(it + 1));
					if(var < variables.size())
						stack.push(variables[var]);

					it += 2;
					break;
				}

				case Instruction::Pop:
				{
					int max = static_cast<unsigned int>(*(it + 1));
					
					for(int pops = 0; pops < max; pops++)
						stack.pop();

					it += 2;
					break;
				}

				case Instruction::Store:
					if(!stack.empty())
					{
						unsigned int var = *(it + 1);
						if(var < variables.size())
							variables[var] = const_cast<BaseValue*>(stack.top());
						else
							variables.push_back(const_cast<BaseValue*>(stack.top()));
						stack.pop();
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					it += 2;
					break;

				case Instruction::Erase:
					if(!variables.empty() && *(it + 1) < variables.size())
						variables.erase(variables.begin() + *(it + 1));
					else
						throw std::runtime_error("No variable stored at: " + *(it + 1) + std::string(". OP: ") + std::to_string(it - bytecode.begin()));

					it += 2;
					break;

				case Instruction::Add:
				{
					Value<double> one;
					Value<double> two;
					
					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
					{
						std::cerr << "Stack too small. OP: " << std::distance(bytecode.begin(), it) << '\n';
						throw std::runtime_error("");
					}

					stack.push(new Value<double>(two.getValue() + one.getValue()));

					it++;
					break;
				}

				case Instruction::Sub:
				{
					Value<double> one;
					Value<double> two;

					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
					{
						std::cerr << "Stack too small. OP: " << std::distance(bytecode.begin(), it) << '\n';
						throw std::runtime_error("");
					}

					stack.push(new Value<double>(two.getValue() - one.getValue()));

					it++;
					break;
				}

				case Instruction::Mult:
				{
					Value<double> one;
					Value<double> two;

					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
					{
						std::cerr << "Stack too small. OP: " << std::distance(bytecode.begin(), it) << '\n';
						throw std::runtime_error("");
					}

					stack.push(new Value<double>(two.getValue() * one.getValue()));

					it++;
					break;
				}

				case Instruction::Div:
				{
					Value<double> one;
					Value<double> two;

					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
					{
						std::cerr << "Stack too small. OP: " << std::distance(bytecode.begin(), it) << '\n';
						throw std::runtime_error("");
					}

					stack.push(new Value<double>(two.getValue() / one.getValue()));

					it++;
					break;
				}

				case Instruction::Mod:
				{
					Value<double> one;
					Value<double> two;

					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
					{
						std::cerr << "Stack too small. OP: " << std::distance(bytecode.begin(), it) << '\n';
						throw std::runtime_error("");
					}

					stack.emplace(new Value<double>(static_cast<int>(two.getValue()) % static_cast<int>(one.getValue())));

					it++;
					break;
				}

				case Instruction::Greater:
				{
					Value<double> one;
					Value<double> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + std::distance(bytecode.begin(), it));

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + std::distance(bytecode.begin(), it));
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + std::distance(bytecode.begin(), it));

					stack.emplace(new Value<bool>(one.getValue() > two.getValue()));

					it++;
					break;
				}

				case Instruction::Lesser:
				{
					Value<double> one;
					Value<double> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					stack.emplace(new Value<bool>(one.getValue() < two.getValue()));

					it++;
					break;
				}

				case Instruction::GreaterOrEqual:
				{
					Value<double> one;
					Value<double> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					stack.emplace(new Value<bool>(one.getValue() >= two.getValue()));

					it++;
					break;
				}

				case Instruction::LesserOrEqual:
				{
					Value<double> one;
					Value<double> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					stack.emplace(new Value<bool>(one.getValue() <= two.getValue()));

					it++;
					break;
				}

				case Instruction::Equal:
				{
					Value<double> one;
					Value<double> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					stack.emplace(new Value<bool>(one.getValue() == two.getValue()));

					it++;
					break;
				}

				case Instruction::NotEqual:
				{
					Value<double> one;
					Value<double> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<double>*>(stack.top());
							stack.pop();
						}
						else
							throw std::runtime_error("Value on stack is not a number. OP: " + (it - bytecode.begin()));
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					stack.emplace(new Value<bool>(one.getValue() != two.getValue()));

					it++;
					break;
				}

				case Instruction::If:
					if(stack.top()->getValueType() == ValueType::Bool)
					{
						if(static_cast<const Value<bool>*>(stack.top())->getValue())
							it = bytecode.begin() + static_cast<unsigned int>(*(it + 1));
						else
							it = bytecode.begin() + static_cast<unsigned int>(*(it + 2));
					}
					else
						throw std::runtime_error("Value on top of stack does not evaluate to bool. OP: " + (it - bytecode.begin()));
					break;

				case Instruction::Goto:
					previous = it;
					it = bytecode.begin() + static_cast<unsigned int>(*(it + 1));
					break;

				case Instruction::Return:
					it = previous + 1;
					break;

				case Instruction::End:
					if(!stack.empty())
						std::cerr << "Result: " << static_cast<const Value<double>*>(stack.top())->getValue() << '\n';
					else
						std::cerr << "Variable: " << static_cast<const Value<double>*>(variables.back())->getValue() << '\n';
					while(!stack.empty())
						stack.pop();

					variables.clear();
					it = bytecode.end();
					break;

				default:
					it++;
					break;
			}
		}
	}

	unsigned int VM::getStackSize() const
	{
		return stack.size();
	}
}
