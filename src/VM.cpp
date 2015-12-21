#include "VM.hpp"

#include <cctype>
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

		return {left, right};
	}

	static std::size_t toRegister(const std::string& regStr)
	{
		if(regStr[0] == '$')
			return std::stoi(regStr.substr(1));
		else
			throw std::exception("Registry indices must start with '$'");
	}
}

namespace dbr
{
	namespace svm
	{
		void VM::run(const Bytecode& bytecode)
		{
			// read and load constants
			// read and load functions

			stack.emplace();

			StackFrame* currFrame = &stack.top();

		}

		void VM::interpreter(std::istream& in, std::ostream& out)
		{
			stack.emplace();

			StackFrame* currFrame = &stack.top();

			std::string line;

			out << "SomeVM (C) Alec Iverson, 2015\n> ";
			std::getline(in, line);

			do
			{
				std::string command;

				std::istringstream iss(line);
				iss >> command;

				toLower(command);

				try
				{
					Instruction instruction;

					if(command == "print")
					{
						std::string regStr;
						iss >> regStr;

						std::size_t reg = toRegister(regStr);

						Value val = currFrame->read(reg);

						switch(val.getType())
						{
							case Value::Type::None:
								out << "none";
								break;

							case Value::Type::Bool:
								out << "bool: " << std::boolalpha << static_cast<bool>(val);
								break;

							case Value::Type::Number:
								out << "number: " << static_cast<number>(val);
								break;

							case Value::Type::String:
								out << "string: \"" << static_cast<string>(val) << '"';
								break;
						}
					}
					// VM instructions
					else if(command == "load")
					{
						std::string regStr;
						std::string value;
						iss >> regStr;

						std::getline(iss, value);

						std::uint8_t reg = toRegister(regStr);
						value = stringTrim(value);

						Value val;
						std::uint16_t constIdx = 0;

						if(value[0] == '$')
						{
							constIdx = static_cast<std::uint16_t>(toRegister(value));
						}
						else if(value[0] == '"' && value.back() == '"')
						{
							val.set(value.substr(1, value.length() - 2));

							constants.push_back(val);
							constIdx = static_cast<std::uint16_t>(constants.size() - 1);
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
							constIdx = static_cast<std::uint16_t>(constants.size() - 1);
						}

						instruction = {Instruction::Type::Load, reg, constIdx};
					}
					else if(command == "add")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::Add, writeTo, one, two};
					}
					else if(command == "sub")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::Subtract, writeTo, one, two};
					}
					else if(command == "mult")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::Multiply, writeTo, one, two};
					}
					else if(command == "div")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::Divide, writeTo, one, two};
					}
					else if(command == "mod")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::Modulus, writeTo, one, two};
					}
					else if(command == "not")
					{
						std::string writeToStr;
						std::string oneStr;

						iss >> writeToStr >> oneStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));

						instruction = {Instruction::Type::Not, writeTo, one, 0};
					}
					else if(command == "less")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::Lesser, writeTo, one, two};
					}
					else if(command == "lesseq")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::LesserOrEqual, writeTo, one, two};
					}
					else if(command == "gret")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::Greater, writeTo, one, two};
					}
					else if(command == "greteq")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::GreaterOrEqual, writeTo, one, two};
					}
					else if(command == "eq")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::IsEqual, writeTo, one, two};
					}
					else if(command == "neq")
					{
						std::string writeToStr;
						std::string oneStr;
						std::string twoStr;

						iss >> writeToStr >> oneStr >> twoStr;

						auto writeTo = static_cast<std::uint8_t>(toRegister(writeToStr));
						auto one = static_cast<std::uint8_t>(toRegister(oneStr));
						auto two = static_cast<std::uint8_t>(toRegister(twoStr));

						instruction = {Instruction::Type::NotEqual, writeTo, one, two};
					}
					else
					{
						out << "Unknown command!";
					}

					if(instruction.type() != Instruction::Type::Noop)
					{
						currFrame->push(instruction);
						bool state = currFrame->run(constants, functions);
					}
				}
				catch(const std::exception& e)
				{
					out << "\nError: " << e.what() << std::endl;
				}

				out << "\n> ";
				std::getline(in, line);
			}
			while(line != "exit");
		}

		std::size_t VM::stackSize() const
		{
			return stack.size();
		}
	}
}
