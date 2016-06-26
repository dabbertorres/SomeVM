#include "Assembler.hpp"

#include <algorithm>

#include <string>
#include <sstream>

#include "Function.hpp"
#include "Program.hpp"
#include "VM.hpp"

#include "Util.hpp"

namespace
{
	using namespace dbr::svm;

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
		std::string oneStr;
		std::string twoStr;

		in >> oneStr >> twoStr;

		std::uint8_t one = Assembler::toRegister(oneStr);
		std::uint16_t two = Assembler::toRegister(twoStr);

		return{type, one, two};
	}

	static Instruction threeArg(std::istream& in, Instruction::Type type)
	{
		std::string oneStr;
		std::string twoStr;
		std::string threeStr;

		in >> oneStr >> twoStr >> threeStr;

		auto one = static_cast<std::uint8_t>(Assembler::toRegister(oneStr));
		auto two = static_cast<std::uint8_t>(Assembler::toRegister(twoStr));
		auto three = static_cast<std::uint8_t>(Assembler::toRegister(threeStr));

		return{type, one, two, three};
	}

	static void constant(std::istream& in, dbr::svm::Constants& constants)
	{
		std::string value;
		std::getline(in, value);
		value = util::stringTrim(value);

		dbr::svm::Value val;

		// string value
		if(value[0] == '"' && value.back() == '"')
		{
			val.set(value.substr(1, value.length() - 2));	// "- 2" do not include the quotes

			constants.push_back(val);
		}
		else
		{
			util::toLower(value);

			if(util::isBool(value))
			{
				val.set(util::strToBool(value));
			}
			else if(util::isInt(value))
			{
				val.set(std::stoll(value));
			}
			else if(util::isFloat(value))
			{
				val.set(std::stod(value));
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

			std::stack<Function> codeStack;

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

				util::toLower(command);

				try
				{
					auto it = commands.end();

					if(codeStack.empty())
						throw std::runtime_error("Unexpected end of function");

					auto& top = codeStack.top();

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
						top.code.push_back(inst);
					}
					// start function
					else if(command.back() == ':')
					{
						// number of returns is the first number following the colon
						// number of arguments is the second number following the colon (with a space between them! (strict for now...))
						std::uint32_t numRets = 0;
						std::uint32_t numArgs = 0;
						iss >> numRets >> numArgs;

						codeStack.emplace(static_cast<std::uint8_t>(numRets), static_cast<std::uint8_t>(numArgs), Bytecode{});
					}
					// end function
					else if(command == "end")
					{
						program.functions.push_back(top);
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
				program.functions.insert(program.functions.begin(), codeStack.top());
				codeStack.pop();
			}
		}

		bool Assembler::isRegister(const std::string& str)
		{
			if(str[0] != '$')
				return false;

			auto rest = str.substr(1);

			// check decimal
			if(util::isInt(rest))
				return true;

			if(util::isHex(rest))
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

			/* conditional branching */
			{"jmpt", [](std::istream& in) { return twoArg(in, Instruction::Type::JmpT); }},
			{"jmpf", [](std::istream& in) { return twoArg(in, Instruction::Type::JmpF); }},

			{"rjmpt", [](std::istream& in) { return twoArg(in, Instruction::Type::RJmpT); }},
			{"rjmpf", [](std::istream& in) { return twoArg(in, Instruction::Type::RJmpF); }},

			/* branching */
			{"call", [](std::istream& in) { return threeArg(in, Instruction::Type::Call); }},
			{"ret", [](std::istream& in) { return twoArg(in, Instruction::Type::Ret); }},
			{"jmp", [](std::istream& in) { return oneArgX(in, Instruction::Type::Jmp); }},
			{"rjmp", [](std::istream& in) { return oneArgX(in, Instruction::Type::RJmp); }},

			/* misc */
			{"nop", [](std::istream& in) { return Instruction(Instruction::Type::Nop, 0); }},
			{"print", [](std::istream& in) { return oneArg(in, Instruction::Type::Print); }},
		};
	}
}
