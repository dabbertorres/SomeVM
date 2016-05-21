#include "Assembler.hpp"

#include <cctype>
#include <algorithm>

#include <string>
#include <sstream>

#include "VM.hpp"

namespace
{
	using namespace dbr;

	static void toLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	}

	static std::string stringTrim(const std::string& str)
	{
		auto left = std::find_if_not(str.begin(), str.end(), std::isspace);
		auto right = std::find_if_not(str.rbegin(), str.rend(), std::isspace).base();

		return{left, right};
	}

	using Instruction = dbr::svm::Instruction;
	using Assembler = dbr::svm::Assembler;

	static Instruction oneArg(std::istream& in, Instruction::Type type)
	{
		std::string oneStr;

		in >> oneStr;

		auto one = static_cast<std::uint8_t>(Assembler::toRegister(oneStr));

		return{type, one, 0, 0};
	}

	static Instruction oneArgX(std::istream& in, Instruction::Type type)
	{
		std::string oneStr;

		in >> oneStr;

		auto one = static_cast<std::uint32_t>(Assembler::toRegister(oneStr));

		return{type, one};
	}

	static Instruction twoArg(std::istream& in, Instruction::Type type)
	{
		std::string writeToStr;
		std::string oneStr;

		in >> writeToStr >> oneStr;

		std::uint8_t writeTo = Assembler::toRegister(writeToStr);
		std::uint16_t one = Assembler::toRegister(oneStr);

		return{type, writeTo, one};
	}

	static Instruction threeArg(std::istream& in, Instruction::Type type)
	{
		std::string writeToStr;
		std::string oneStr;
		std::string twoStr;

		in >> writeToStr >> oneStr >> twoStr;

		auto writeTo = static_cast<std::uint8_t>(Assembler::toRegister(writeToStr));
		auto one = static_cast<std::uint8_t>(Assembler::toRegister(oneStr));
		auto two = static_cast<std::uint8_t>(Assembler::toRegister(twoStr));

		return{type, writeTo, one, two};
	}

	static void constant(std::istream& in, dbr::svm::Constants& constants)
	{
		std::string value;
		std::getline(in, value);
		value = stringTrim(value);

		dbr::svm::Value val;

		// string value
		if(value[0] == '"' && value.back() == '"')
		{
			val.set(value.substr(1, value.length() - 2));	// "- 2" do not include the quotes

			constants.push_back(val);
		}
		else
		{
			toLower(value);

			svm::Bool b;
			svm::Int i;
			svm::Float f;

			if(Assembler::strToBool(value, b))
			{
				val.set(b);
			}
			else if(Assembler::strToInt(value, i))
			{
				val.set(i);
			}
			else if(Assembler::strToFloat(value, f))
			{
				val.set(f);
			}
			else
			{
				std::string msg = "Unrecognizable value \"";
				msg += value;
				msg += '"';

				throw std::runtime_error(msg);
			}

			constants.push_back(val);
		}
	}
}

namespace dbr
{
	namespace svm
	{
		Program Assembler::run(std::istream& in, std::ostream& out)
		{
			Program program;

			run(in, out, program);

			return program;
		}

		void Assembler::run(std::istream& in, std::ostream& out, Program& program)
		{
			std::size_t lineNum = 1;
			std::string line;

			// stack of number of arguments and Bytecode for each StackFrame
			struct Frame
			{
				std::size_t nrets;
				std::size_t nargs;
				Bytecode code;

				Frame() = default;
				Frame(std::size_t nrets, std::size_t nargs, Bytecode code)
					: nrets(nrets),
					nargs(nargs),
					code(code)
				{}
			};

			std::stack<Frame> codeStack;

			// top level function has no returns and 1 argument (an array of command-line parameters) (or, will at least)
			codeStack.emplace(0, 1, Bytecode{});

			for(; std::getline(in, line); ++lineNum)
			{
				if(line.empty())
					continue;

				std::string command;

				std::istringstream iss(line);
				iss >> command;

				if(command.empty())
					continue;

				toLower(command);

				try
				{
					auto it = commands.end();

					if(codeStack.empty())
						throw std::runtime_error("Unexpected end of function");

					auto* top = &codeStack.top();

					// comment
					if(command[0] == '#')
					{
						continue;
					}
					// const declaration
					else if(command == "const")
					{
						constant(iss, program.constants);
					}
					// command
					else if((it = commands.find(command)) != commands.end())
					{
						auto inst = it->second(iss);
						top->code.push_back(inst);
					}
					// start function
					else if(command.back() == ':')
					{
						// number of returns is the first number following the colon
						// number of arguments is the second number following the colon (with a space between them! (strict for now...))
						std::size_t numRets = 0;
						std::size_t numArgs = 0;
						iss >> numRets >> numArgs;

						codeStack.emplace(numRets, numArgs, Bytecode{});
					}
					// end function
					else if(command == "end")
					{
						program.functions.emplace_back(top->nrets, top->nargs, StackFrame(top->code));
						codeStack.pop();
					}
					else
					{
						out << "Unknown command \"" << command << "\"!";
					}
				}
				catch(const std::exception& e)
				{
					out << "\nError (line: " << lineNum << "): " << e.what() << std::endl;
					throw e;
				}
			}

			// if the top-level function was not popped
			if(!codeStack.empty())
			{
				auto& top = codeStack.top();
				program.functions.emplace(program.functions.begin(), top.nrets, top.nargs, StackFrame(top.code));
				codeStack.pop();
			}
		}

		bool Assembler::isRegister(const std::string& str)
		{
			if(str[0] != '$')
				return false;

			auto rest = str.substr(1);

			// check decimal
			if(isInt(rest))
				return true;

			if(isHex(rest))
				return true;

			return false;
		}

		std::uint8_t Assembler::toRegister(const std::string& regStr)
		{
			if(!isRegister(regStr))
			{
				std::string msg = "Expected register prefix '$', got \'";
				msg += regStr[0];
				msg += '\'';

				throw std::runtime_error(msg);
			}

			return std::stoi(regStr.substr(1));
		}

		bool Assembler::isInt(const std::string& str)
		{
			auto it = str.begin();

			if(!std::isdigit(*it) && *it != '-' && *it != '+')
				return false;

			for(++it; it != str.end(); ++it)
			{
				if(!std::isdigit(*it))
					return false;
			}

			return true;
		}

		bool Assembler::isFloat(const std::string& str)
		{
			auto it = str.begin();

			if(!std::isdigit(*it) && *it != '-' && *it != '.' && *it != '+')
				return false;

			// enforcing floats to actually need a decimal point
			if(str.find('.') == std::string::npos)
				return false;

			for(++it; it != str.end(); ++it)
			{
				if(!std::isdigit(*it) && *it != '.')
					return false;
			}

			return true;
		}

		bool Assembler::isBool(const std::string& str)
		{
			return str == "true" || str == "false";
		}

		bool Assembler::isHex(const std::string& str)
		{
			if(str[0] != '0' || str[1] != 'x')
				return false;

			for(auto it = str.begin() + 2; it != str.end(); ++it)
			{
				if(!std::isxdigit(*it))
					return false;
			}

			return true;
		}

		bool Assembler::strToInt(const std::string& str, Int& i)
		{
			if(!isInt(str))
				return false;

			auto it = str.begin();

			bool neg = *it == '-';
			bool pos = *it == '+';
			bool sign = neg || pos;

			auto place = str.length() - 1 - sign;	// "- 1" for base 0, and subtract another if a positive/negative character exists
			i = 0;

			if(sign)
				++it;

			for(; it != str.end(); ++it, --place)
			{
				i += (*it - '0') * static_cast<std::size_t>(std::pow(10, place));
			}

			if(neg)
				i *= -1;

			return true;
		}

		bool Assembler::strToFloat(const std::string& str, Float& f)
		{
			if(!isFloat(str))
				return false;

			auto it = str.begin();

			bool neg = *it == '-';
			bool pos = *it == '+';
			bool sign = neg || pos;

			auto decimalPos = str.find('.');
			auto place = str.length() - decimalPos - 1 - sign;	// "- 1" for base 0, and subtract another if a positive/negative character exists
			f = 0;

			if(sign)
				++it;

			for(; it != str.end(); ++it, --place)
			{
				if(*it == '.')
					continue;

				f += (*it - '0') * static_cast<float>(std::pow(10, place));
			}

			if(neg)
				f *= -1;

			return true;
		}

		bool Assembler::strToBool(const std::string& str, Bool& b)
		{
			if(!isBool(str))
				return false;

			b = str[0] == 't';

			return true;
		}

		const std::unordered_map<std::string, Instruction(*)(std::istream&)> Assembler::commands =
		{
			/* memory ops */
			{"load", [](std::istream& in) { return twoArg(in, Instruction::Type::Load); }},
			{"loadc", [](std::istream& in) { return twoArg(in, Instruction::Type::LoadC); }},

			/* math ops */
			// integer
			{"add", [](std::istream& in) { return threeArg(in, Instruction::Type::Add); }},
			{"sub", [](std::istream& in) { return threeArg(in, Instruction::Type::Sub); }},
			{"mult", [](std::istream& in) { return threeArg(in, Instruction::Type::Mult); }},
			{"div", [](std::istream& in) { return threeArg(in, Instruction::Type::Div); }},
			{"mod", [](std::istream& in) { return threeArg(in, Instruction::Type::Mod); }},
			{"neg", [](std::istream& in) { return twoArg(in, Instruction::Type::Neg); }},

			// float
			{"fadd", [](std::istream& in) { return threeArg(in, Instruction::Type::FAdd); }},
			{"fsub", [](std::istream& in) { return threeArg(in, Instruction::Type::FSub); }},
			{"fmult", [](std::istream& in) { return threeArg(in, Instruction::Type::FMult); }},
			{"fdiv", [](std::istream& in) { return threeArg(in, Instruction::Type::FDiv); }},
			{"fneg", [](std::istream& in) { return twoArg(in, Instruction::Type::FNeg); }},

			// misc
			{"casti", [](std::istream& in) { return twoArg(in, Instruction::Type::CastI); }},
			{"castf", [](std::istream& in) { return twoArg(in, Instruction::Type::CastF); }},

			/* comparison ops */
			// integer
			{"lt", [](std::istream& in) { return threeArg(in, Instruction::Type::Lt); }},
			{"lteq", [](std::istream& in) { return threeArg(in, Instruction::Type::LtEq); }},
			{"gt", [](std::istream& in) { return threeArg(in, Instruction::Type::Gt); }},
			{"gteq", [](std::istream& in) { return threeArg(in, Instruction::Type::GtEq); }},
			{"eq", [](std::istream& in) { return threeArg(in, Instruction::Type::Eq); }},
			{"neq", [](std::istream& in) { return threeArg(in, Instruction::Type::Neq); }},

			// float
			{"flt", [](std::istream& in) { return threeArg(in, Instruction::Type::Lt); }},
			{"flteq", [](std::istream& in) { return threeArg(in, Instruction::Type::LtEq); }},
			{"fgt", [](std::istream& in) { return threeArg(in, Instruction::Type::Gt); }},
			{"fgteq", [](std::istream& in) { return threeArg(in, Instruction::Type::GtEq); }},
			{"feq", [](std::istream& in) { return threeArg(in, Instruction::Type::Eq); }},
			{"fneq", [](std::istream& in) { return threeArg(in, Instruction::Type::Neq); }},

			/* logical ops */
			{"not", [](std::istream& in) { return twoArg(in, Instruction::Type::Not); }},
			{"and", [](std::istream& in) { return threeArg(in, Instruction::Type::Add); }},
			{"or", [](std::istream& in) { return threeArg(in, Instruction::Type::Or); }},
			{"xor", [](std::istream& in) { return threeArg(in, Instruction::Type::Xor); }},

			/* bitwise ops */
			{"bnot", [](std::istream& in) { return twoArg(in, Instruction::Type::BNot); }},
			{"band", [](std::istream& in) { return threeArg(in, Instruction::Type::BAnd); }},
			{"bor", [](std::istream& in) { return threeArg(in, Instruction::Type::BOr); }},
			{"bxor", [](std::istream& in) { return threeArg(in, Instruction::Type::BXor); }},
			{"bsl", [](std::istream& in) { return threeArg(in, Instruction::Type::Bsl); }},
			{"bsr", [](std::istream& in) { return threeArg(in, Instruction::Type::Bsr); }},

			/* branching */
			{"jmpt", [](std::istream& in) { return oneArgX(in, Instruction::Type::JmpT); }},
			{"jmpf", [](std::istream& in) { return oneArgX(in, Instruction::Type::JmpF); }},

			/* jumps */
			{"call", [](std::istream& in)
						{
							std::string numArgsStr;
							std::string funcRegStr;

							in >> numArgsStr >> funcRegStr;

							Int numArgs = 0;
							if(!strToInt(numArgsStr, numArgs))
								throw std::runtime_error("Invalid argument #0 to call (must be positive int)");

							auto funcReg = toRegister(funcRegStr);

							return Instruction(Instruction::Type::Call, static_cast<std::uint8_t>(numArgs), funcReg, 0);
						}
			},
			{"ret", [](std::istream& in) { return twoArg(in, Instruction::Type::Ret); }},
			{"jump", [](std::istream& in)
						{
							// 24 bit maxes/mins
							constexpr auto maxUnsigned = 0xffffff;
							constexpr auto maxSigned = maxUnsigned / 2;
							constexpr auto minSigned = maxSigned - maxUnsigned;

							std::string jumpOffStr;

							in >> jumpOffStr;

							Int jumpOff;
							if(!strToInt(jumpOffStr, jumpOff))
								throw std::runtime_error("Invalid argument to jump (must be an int)");

							// check bounds
							if(jumpOff < minSigned || jumpOff > maxSigned)
								throw std::runtime_error("Invalid argument to jump (Out of bounds! Must fit in (signed) 24 bits)");

							return Instruction(Instruction::Type::Jump, static_cast<std::uint32_t>(jumpOff));
						}
			},

			/* misc */
			{"nop", [](std::istream& in) { return Instruction(Instruction::Type::Nop, 0); }},
			{"print", [](std::istream& in) { return oneArg(in, Instruction::Type::Print); }},
		};
	}
}
