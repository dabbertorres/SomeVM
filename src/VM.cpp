#include "VM.hpp"

#include <cctype>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "Assembler.hpp"

namespace
{
	static void toLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	}
}

namespace dbr
{
	namespace svm
	{
		VM::VM(std::istream& in, std::ostream& out)
		:	in(in),
			out(out)
		{}

		Program VM::loadBinary(const std::string& file)
		{
			std::ifstream fin(file, std::ios::binary);

			if(!fin)
				throw std::runtime_error("Unable to open input file");

			std::string identifier(4, 0);
			fin.read(&identifier[0], strLen(BINARY_ID));

			if(identifier != BINARY_ID)
				throw std::runtime_error("Input file is not a valid svm binary");

			std::uint32_t version = 0;
			fin.read(reinterpret_cast<char*>(&version), sizeof(version));

			// check version...
			/*
			if(version != thisVersion)
				throw std::runtime_error("Incompatible version!");
			*/

			Program out;

			// constants
			std::uint32_t numConstants = 0;
			fin.read(reinterpret_cast<char*>(&numConstants), sizeof(numConstants));

			for(std::uint32_t i = 0; i < numConstants; ++i)
			{
				Value::Type type;
				fin.read(reinterpret_cast<char*>(&type), sizeof(type));

				switch(type)
				{
					case Value::Type::Nil:
						out.constants.emplace_back();
						break;

					case Value::Type::Bool:
					{
						bool val;
						fin.read(reinterpret_cast<char*>(&val), sizeof(val));
						out.constants.emplace_back(val);
						break;
					}

					case Value::Type::Int:
					{
						int val;
						fin.read(reinterpret_cast<char*>(&val), sizeof(val));
						out.constants.emplace_back(val);
						break;
					}

					case Value::Type::Float:
					{
						float val;
						fin.read(reinterpret_cast<char*>(&val), sizeof(val));
						out.constants.emplace_back(val);
						break;
					}

					case Value::Type::String:
					{
						std::size_t len = 0;
						fin.read(reinterpret_cast<char*>(&len), sizeof(len));

						string val(len, 0);
						fin.read(reinterpret_cast<char*>(&val[0]), len);

						out.constants.emplace_back(val);
						break;
					}

					default:
					{
						std::string msg = "Unknown type: ";
						msg += static_cast<byte>(type);

						throw std::runtime_error(msg);
					}
				}
			}

			// functions
			std::uint32_t numFunctions = 0;
			fin.read(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

			for(std::uint32_t i = 0; i < numFunctions; ++i)
			{
				// number of arguments the function takes
				std::uint8_t nargs;

				// length of the function in Instructions
				std::uint32_t numInstrs;

				fin.read(reinterpret_cast<char*>(&nargs), sizeof(nargs));
				fin.read(reinterpret_cast<char*>(&numInstrs), sizeof(numInstrs));

				Bytecode code(numInstrs);

				fin.read(reinterpret_cast<char*>(code.data()), numInstrs * sizeof(Instruction));

				out.functions.emplace_back(nargs, StackFrame(std::move(code)));
			}

			return out;
		}

		void VM::writeBinary(const Program& program, const std::string& file)
		{
			std::ofstream fout(file, std::ios::binary);

			if(!fout)
				throw std::runtime_error("Unable to open output file");

			// file type identifier
			fout.write(BINARY_ID, strLen(BINARY_ID));

			// version
			fout.write("0000", 4);	// heh. need to figure out version stuff.

			// constants
			std::uint32_t numConstants = program.constants.size();
			fout.write(reinterpret_cast<char*>(&numConstants), sizeof(numConstants));

			for(auto& c : program.constants)
			{
				auto type = c.type();

				fout.write(reinterpret_cast<char*>(&type), sizeof(type));

				if(type != Value::Type::String)
				{
					Bytes bytes = c;
					fout.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
				}
				else
				{
					string str = c;
					std::size_t len = str.length();

					fout.write(reinterpret_cast<char*>(&len), sizeof(len));
					fout.write(str.c_str(), len);
				}
			}

			// functions
			std::uint32_t numFunctions = program.functions.size();
			fout.write(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

			for(auto& f : program.functions)
			{
				fout.write(reinterpret_cast<const char*>(&f.first), sizeof(f.first));
				
				auto& code = f.second.getBytecode();
				std::uint32_t numInstrs = code.size();

				fout.write(reinterpret_cast<const char*>(&numInstrs), sizeof(numInstrs));

				fout.write(reinterpret_cast<const char*>(code.data()), numInstrs * sizeof(Instruction));
			}
		}

		void VM::run(const Program& program)
		{
			callStack.push(program.functions.front().second);

			while(!callStack.empty())
			{
				StackFrame& currFrame = callStack.top();

				auto currInstr = currFrame.next();

				if(currInstr != currFrame.codeEnd())
					interpret(program, currFrame, currInstr);
				else
					callStack.pop();
			}
		}

		void VM::repl()
		{
			Program program;

			callStack.emplace();

			out << "SomeVM";

			std::string line;

			while(line != "exit")
			{
				out << "\n> ";
				std::getline(in, line);
				
				try
				{
					std::istringstream iss(line);

					il::Assembler::run(iss, out, program);

					StackFrame& currFrame = callStack.top();

					Bytecode::const_iterator next = currFrame.next();

					interpret(program, currFrame, next);

					if(callStack.empty())
						callStack.emplace();
			}
				catch(const std::exception& e)
				{
					out << "\nError: " << e.what() << std::endl;
				}
			}
		}

		std::size_t VM::callStackSize() const
		{
			return callStack.size();
		}

		void VM::interpret(const Program& program, StackFrame& currFrame, Bytecode::const_iterator& currInstr)
		{
			switch(currInstr->type())
			{
				/* memory ops */
				case Instruction::Type::Load:
				{
					currFrame.write(currInstr->arg1(), currFrame.read(currInstr->arg2()));
					break;
				}

				case Instruction::Type::LoadC:
				{
					currFrame.write(currInstr->arg1(), program.constants[currInstr->arg2x()]);
					break;
				}

				/* math ops */
				// integer
				case Instruction::Type::Add:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one + two});
					break;
				}

				case Instruction::Type::Sub:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one - two});
					break;
				}

				case Instruction::Type::Mult:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one * two});
					break;
				}

				case Instruction::Type::Div:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one / two});
					break;
				}

				case Instruction::Type::Mod:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one % two});
					break;
				}

				case Instruction::Type::Neg:
				{
					int one = currFrame.read(currInstr->arg2());

					currFrame.write(currInstr->arg1(), {-one});
					break;
				}

				// float
				case Instruction::Type::FAdd:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one + two});
					break;
				}

				case Instruction::Type::FSub:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one - two});
					break;
				}

				case Instruction::Type::FMult:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one * two});
					break;
				}

				case Instruction::Type::FDiv:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one / two});
					break;
				}

				case Instruction::Type::FNeg:
				{
					float one = currFrame.read(currInstr->arg2());

					currFrame.write(currInstr->arg1(), {-one});
					break;
				}

				// misc
				case Instruction::Type::CastI:
				{
					float one = currFrame.read(currInstr->arg2());

					currFrame.write(currInstr->arg1(), {static_cast<int>(one)});
					break;
				}

				case Instruction::Type::CastF:
				{
					int one = currFrame.read(currInstr->arg2());

					currFrame.write(currInstr->arg1(), {static_cast<float>(one)});
					break;
				}

				/* comparison ops */
				// integer
				case Instruction::Type::Lt:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one < two});
					break;
				}

				case Instruction::Type::LtEq:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one <= two});
					break;
				}

				case Instruction::Type::Gt:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one > two});
					break;
				}

				case Instruction::Type::GtEq:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one >= two});
					break;
				}

				case Instruction::Type::Eq:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one == two});
					break;
				}

				case Instruction::Type::Neq:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one != two});
					break;
				}

				// float
				case Instruction::Type::FLt:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one < two});
					break;
				}

				case Instruction::Type::FLtEq:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one <= two});
					break;
				}

				case Instruction::Type::FGt:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one > two});
					break;
				}

				case Instruction::Type::FGtEq:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one >= two});
					break;
				}

				case Instruction::Type::FEq:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one == two});
					break;
				}

				case Instruction::Type::FNeq:
				{
					float one = currFrame.read(currInstr->arg2());
					float two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one != two});
					break;
				}

				/* logical ops */
				case Instruction::Type::Not:
				{
					bool one = currFrame.read(currInstr->arg2());

					currFrame.write(currInstr->arg1(), {!one});
					break;
				}

				case Instruction::Type::And:
				{
					bool one = currFrame.read(currInstr->arg2());
					bool two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one && two});
					break;
				}

				case Instruction::Type::Or:
				{
					bool one = currFrame.read(currInstr->arg2());
					bool two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one || two});
					break;
				}

				case Instruction::Type::Xor:
				{
					bool one = currFrame.read(currInstr->arg2());
					bool two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one != two});
					break;
				}

				/* bitwise ops */
				case Instruction::Type::BAnd:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one & two});
					break;
				}

				case Instruction::Type::BOr:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one | two});
					break;
				}

				case Instruction::Type::BXor:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one ^ two});
					break;
				}

				case Instruction::Type::Bsl:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one << two});
					break;
				}

				case Instruction::Type::Bsr:
				{
					int one = currFrame.read(currInstr->arg2());
					int two = currFrame.read(currInstr->arg3());

					currFrame.write(currInstr->arg1(), {one >> two});
					break;
				}

				/* conditions */
				case Instruction::Type::If:
				{
					bool one = currFrame.read(currInstr->arg1x());

					// if true, skip the next instruction (the jump to the "else")
					if(one)
						currFrame.jump(currInstr - currFrame.codeBegin() + 2);	// add 2 to skip over the current, and next instructions

					break;
				}

				/* branching */
				case Instruction::Type::Call:
				{
					std::uint8_t nargs = currInstr->arg1();
					Function callee = program.functions[currInstr->arg2x()];

					if(nargs != callee.first)
						throw std::runtime_error("Invalid number of arguments!");

					auto beg = currFrame.regBegin();
					callee.second.writeAll(beg, beg + nargs);

					callStack.push(callee.second);

					break;
				}

				case Instruction::Type::Ret:
				{
					auto twoX = currInstr->arg2x();

					if(twoX <= 0xFF)
					{
						auto one = currInstr->arg1();

						// "+ 1" for twoX being inclusive
						returns.resize(twoX - one + 1);

						// copy returns out
						auto& registry = currFrame.getRegistry();
						std::copy(registry.begin() + one, registry.begin() + twoX, returns.begin());
					}
					else
					{
						returns.clear();
					}

					// if we're returning before the end of this stackframe, make it so
					callStack.pop();

					break;
				}

				case Instruction::Type::Jump:
				{
					int instIdx = currFrame.read(currInstr->arg1x());
					currFrame.jump(instIdx);
					break;
				}

				case Instruction::Type::Noop:
				{
					break;
				}

				case Instruction::Type::Print:
				{
					auto reg = currInstr->arg1();

					const Value& val = currFrame.read(reg);

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

					out << std::endl;

					break;
				}
			}
		}
	}
}
