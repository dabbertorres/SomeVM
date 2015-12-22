#include "Parser.hpp"

#include <cctype>
#include <algorithm>

#include <string>
#include <sstream>

namespace
{
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
}

namespace dbr
{
	namespace svm
	{
		namespace il
		{
			const std::unordered_map<std::string, Instruction(*)(std::istream&)> Parser::commands =
			{
				// memory ops
				{"load", load},

				// math ops
				{"add", add},
				{"sub", sub},
				{"mult", mult},
				{"div", div},
				{"mod", mod},
				{"neg", neg},

				// comparison
				{"not", not},
				{"less", less},
				{"lesseq", lesseq},
				{"gret", gret},
				{"greteq", greteq},
				{"eq", eq},
				{"neq", neq},

				// conditions
				{"if", ifTest},

				// branching
				{"call", call},
				{"ret", ret},
				{"jump", jump},

				// misc
				{"noop", noop},
				{"print", print},
			};

			Program Parser::run(std::istream& in, std::ostream& out)
			{
				Program program;

				run(in, out, program);

				return program;
			}

			void Parser::run(std::istream& in, std::ostream& out, Program& program)
			{
				std::string line;

				while(std::getline(in, line))
				{
					std::string command;

					std::istringstream iss(line);
					iss >> command;

					toLower(command);

					try
					{
						auto it = commands.end();

						// comment
						if(command[0] == '#')
						{
							continue;
						}
						else if(command == "const")
						{
							constant(iss, program.constants);
						}
						else if((it = commands.find(command)) != commands.end())
						{
							auto inst = it->second(iss);
							program.bytecode.push_back(inst);
						}
						else
						{
							out << "Unknown command \"" << command << "\"!";
						}
					}
					catch(const std::exception& e)
					{
						out << "\nError: " << e.what() << std::endl;
					}
				}
			}

			std::size_t Parser::toRegister(const std::string& regStr)
			{
				if(regStr[0] == '$')
					return std::stoi(regStr.substr(1));
				else
					throw std::exception("Registry/Constant indices must start with '$'");
			}

			Instruction Parser::load(std::istream& in)
			{
				std::string regStr;
				std::string constIdxStr;
				in >> regStr >> constIdxStr;

				std::uint8_t reg = toRegister(regStr);
				std::uint16_t constIdx = toRegister(constIdxStr);

				return{Instruction::Type::Load, reg, constIdx};
			}

			void Parser::constant(std::istream& in, Constants& constants)
			{
				std::string value;
				std::getline(in, value);
				value = stringTrim(value);

				Value val;

				if(value[0] == '"' && value.back() == '"')
				{
					val.set(value.substr(1, value.length() - 2));

					constants.push_back(val);
				}
				else
				{
					toLower(value);

					if(value == "true")
						val.set(true);
					else if(value == "false")
						val.set(false);
					else
						val.set(std::stof(value));

					constants.push_back(val);
				}
			}

			// math ops
			Instruction Parser::add(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::Add, writeTo, one, two};
			}

			Instruction Parser::sub(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::Sub, writeTo, one, two};
			}

			Instruction Parser::mult(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::Mult, writeTo, one, two};
			}

			Instruction Parser::div(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::Div, writeTo, one, two};
			}

			Instruction Parser::mod(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::Mod, writeTo, one, two};
			}

			Instruction Parser::neg(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;

				in >> writeToStr >> oneStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));

				return{Instruction::Type::Neg, writeTo, one, 0};
			}

			// comparison
			Instruction Parser::not(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;

				in >> writeToStr >> oneStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));

				return{Instruction::Type::Not, writeTo, one, 0};
			}

			Instruction Parser::less(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::Less, writeTo, one, two};
			}

			Instruction Parser::lesseq(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::LessEq, writeTo, one, two};
			}

			Instruction Parser::gret(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::Gret, writeTo, one, two};
			}

			Instruction Parser::greteq(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::GretEq, writeTo, one, two};
			}

			Instruction Parser::eq(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::Eq, writeTo, one, two};
			}

			Instruction Parser::neq(std::istream& in)
			{
				std::string writeToStr;
				std::string oneStr;
				std::string twoStr;

				in >> writeToStr >> oneStr >> twoStr;

				auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto two = static_cast<std::uint8_t>(toRegister(twoStr));

				return{Instruction::Type::Neq, writeTo, one, two};
			}

			// conditions
			Instruction Parser::ifTest(std::istream& in)
			{
				std::string oneStr;

				in >> oneStr;

				auto one = static_cast<std::uint8_t>(toRegister(oneStr));

				return{Instruction::Type::If, one, 0, 0};
			}

			// branching
			Instruction Parser::call(std::istream& in)
			{
				std::string oneStr;

				in >> oneStr;

				auto one = static_cast<std::uint32_t>(toRegister(oneStr));

				return{Instruction::Type::Call, one};
			}

			Instruction Parser::ret(std::istream& in)
			{
				std::string oneStr;
				std::string twoXStr;

				in >> oneStr >> twoXStr;

				auto one = static_cast<std::uint8_t>(toRegister(oneStr));
				auto twoX = static_cast<std::uint16_t>(toRegister(twoXStr));

				return{Instruction::Type::Ret, one, twoX};
			}

			Instruction Parser::jump(std::istream& in)
			{
				std::string oneStr;

				in >> oneStr;

				auto one = static_cast<std::uint32_t>(toRegister(oneStr));

				return{Instruction::Type::Jump, one};
			}

			// misc
			Instruction Parser::noop(std::istream& in)
			{
				return{Instruction::Type::Noop, 0};
			}

			Instruction Parser::print(std::istream& in)
			{
				std::string oneStr;

				in >> oneStr;

				auto one = static_cast<std::uint8_t>(toRegister(oneStr));

				return{Instruction::Type::Print, one, 0, 0};
			}
		}
	}
}
