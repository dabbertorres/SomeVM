#include "VM.hpp"

#include "Instruction.hpp"

#include <fstream>
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
						{
//							None none;
//							none.value = nullptr;
//							stack.push(new Value<nil>(none.value));
							it += 2;
							break;
						}
						
						case ValueType::Bool:
						{
							Bool b;
							b.bytes[0] = *(it + 2);
							stack.push(new Value<bool>(b.value));
							it += 3;
							break;
						}
						
						case ValueType::Number:
						{
							Float f;
							f.bytes[0] = *(it + 2);
							f.bytes[1] = *(it + 3);
							f.bytes[2] = *(it + 4);
							f.bytes[3] = *(it + 5);
							stack.push(new Value<float>(f.value));
							it += 6;
							break;
						}
						
						default:
						{
							std::cerr << "Invalid Value in bytecode. OP: " << it - bytecode.begin() << '\n';
							throw std::runtime_error("");
							break;
						}
					}
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
					{
						std::cerr << "Stack is empty. OP: " << (it - bytecode.begin()) << '\n';
						throw std::runtime_error("");
					}

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
					Value<float> one;
					Value<float> two;
					
					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
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

					stack.push(new Value<float>(two.getValue() + one.getValue()));

					it++;
					break;
				}

				case Instruction::Sub:
				{
					Value<float> one;
					Value<float> two;

					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
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

					stack.push(new Value<float>(two.getValue() - one.getValue()));

					it++;
					break;
				}

				case Instruction::Mult:
				{
					Value<float> one;
					Value<float> two;

					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
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

					stack.push(new Value<float>(two.getValue() * one.getValue()));

					it++;
					break;
				}

				case Instruction::Div:
				{
					Value<float> one;
					Value<float> two;

					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
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

					stack.push(new Value<float>(two.getValue() / one.getValue()));

					it++;
					break;
				}

				case Instruction::Mod:
				{
					Value<float> one;
					Value<float> two;

					if(stack.size() >= 2)
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
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

					stack.emplace(new Value<float>(static_cast<int>(two.getValue()) % static_cast<int>(one.getValue())));

					it++;
					break;
				}

				case Instruction::Greater:
				{
					Value<float> one;
					Value<float> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
						
						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + std::distance(bytecode.begin(), it));

					stack.emplace(new Value<bool>(two.getValue() > one.getValue()));

					it++;
					break;
				}

				case Instruction::Lesser:
				{
					Value<float> one;
					Value<float> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					stack.emplace(new Value<bool>(two.getValue() < one.getValue()));

					it++;
					break;
				}

				case Instruction::GreaterOrEqual:
				{
					Value<float> one;
					Value<float> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					stack.emplace(new Value<bool>(two.getValue() >= one.getValue()));

					it++;
					break;
				}

				case Instruction::LesserOrEqual:
				{
					Value<float> one;
					Value<float> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					stack.emplace(new Value<bool>(two.getValue() <= one.getValue()));

					it++;
					break;
				}

				case Instruction::Equal:
				{
					Value<float> one;
					Value<float> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
					}
					else
						throw std::runtime_error("Stack is empty. OP: " + (it - bytecode.begin()));

					stack.emplace(new Value<bool>(one.getValue() == two.getValue()));

					it++;
					break;
				}

				case Instruction::NotEqual:
				{
					Value<float> one;
					Value<float> two;

					if(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Number)
						{
							one = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}

						if(stack.top()->getValueType() == ValueType::Number)
						{
							two = *static_cast<const Value<float>*>(stack.top());
							stack.pop();
						}
						else
						{
							std::cerr << "Value on stack is not a number. OP: " << std::distance(bytecode.begin(), it) << '\n';
							throw std::runtime_error("");
						}
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
						
						stack.pop();
					}
					else
					{
						std::cerr << "Value on stack is not a bool. OP: " << std::distance(bytecode.begin(), it) << '\n';
						throw std::runtime_error("");
					}
					break;

				case Instruction::Goto:
					previous = it;
					it = bytecode.begin() + static_cast<unsigned int>(*(it + 1));
					break;

				case Instruction::Return:
					it = previous + 1;
					break;

				case Instruction::End:
					std::cerr << std::boolalpha;
					while(!stack.empty())
					{
						if(stack.top()->getValueType() == ValueType::Bool)
							std::cerr << "Stack: " << static_cast<const Value<bool>*>(stack.top())->getValue() << '\n';
						else
							std::cerr << "Stack: " << static_cast<const Value<float>*>(stack.top())->getValue() << '\n';
						stack.pop();
					}
					for(auto& v : variables)
					{
						if(v->getValueType() == ValueType::Bool)
							std::cerr << "Variable: " << static_cast<const Value<bool>*>(v)->getValue() << '\n';
						else
							std::cerr << "Variable: " << static_cast<const Value<float>*>(v)->getValue() << '\n';
					}
					
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
	
	VM::Bytecode VM::loadBytes(const std::string& f)
	{
		std::ifstream fin(f, std::ios::binary);
		
		if(fin.bad())
			return {};
		
		Bytecode bytecode;
		
		char b = ' ';
		while(fin.read(&b, 1))
			bytecode.push_back(static_cast<byte>(b));
		
		return bytecode;
	}

	unsigned int VM::getStackSize() const
	{
		return stack.size();
	}
}
