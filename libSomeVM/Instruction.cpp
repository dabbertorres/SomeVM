#include "Instruction.hpp"

#include <map>

namespace svm
{
    static const std::map<std::string, Instruction::Type> stringMap = 
    {
        {"load", Instruction::Type::Load},
        {"loadc", Instruction::Type::LoadC},
        {"add", Instruction::Type::Add},
        {"sub", Instruction::Type::Sub},
        {"mult", Instruction::Type::Mult},
        {"div", Instruction::Type::Div},
        {"mod", Instruction::Type::Mod},
        {"neg", Instruction::Type::Neg},
        {"lt", Instruction::Type::Lt},
        {"lteq", Instruction::Type::LtEq},
        {"gt", Instruction::Type::Gt},
        {"gteq", Instruction::Type::GtEq},
        {"eq", Instruction::Type::Eq},
        {"neq", Instruction::Type::Neq},
        {"not", Instruction::Type::Not},
        {"and", Instruction::Type::And},
        {"or", Instruction::Type::Or},
        {"xor", Instruction::Type::Xor},
        {"jmpt", Instruction::Type::JmpT},
        {"jmpf", Instruction::Type::JmpF},
        {"jmptc", Instruction::Type::JmpTC},
        {"jmpfc", Instruction::Type::JmpFC},
        {"rjmpt", Instruction::Type::RJmpT},
        {"rjmpf", Instruction::Type::RJmpF},
        {"rjmptc", Instruction::Type::RJmpTC},
        {"rjmpfc", Instruction::Type::RJmpFC},
        {"call", Instruction::Type::Call},
        {"ret", Instruction::Type::Ret},
        {"jmp", Instruction::Type::Jmp},
        {"rjmp", Instruction::Type::RJmp},
        {"jmpc", Instruction::Type::JmpC},
        {"rjmpc", Instruction::Type::RJmpC},
    };

    bool Instruction::type(const std::string& str, Type& type)
    {
        auto it = stringMap.find(str);
        if(it != stringMap.end())
        {
            type = it->second;
            return true;
        }
        else
        {
            return false;
        }
    }

	Instruction::Instruction()
		: Instruction(Type::Nop, 0)
	{}

	Instruction::Instruction(std::uint64_t val)
		: value(val)
	{}

	Instruction::Instruction(Type t, std::uint64_t one)
		: value(0)
	{
		value = static_cast<std::uint8_t>(t);
		value <<= 56;

		value |= one & 0x00ffffffffffffff;
	}

	Instruction::Instruction(Type t, std::uint32_t one, std::uint32_t two)
		: value(0)
	{
		value = static_cast<std::uint8_t>(t);
		value <<= 56;

		value |= static_cast<std::uint64_t>(one & 0x00ffffff) << 32;

		value |= two;
	}

	Instruction::Instruction(Type t, std::uint16_t one, std::uint16_t two, std::uint16_t three)
		: value(0)
	{
		value = static_cast<std::uint8_t>(t);
		value <<= 56;

		value |= static_cast<std::uint64_t>(one) << 32;
		value |= static_cast<std::uint64_t>(two) << 16;
		value |= three;
	}

	Instruction::Type Instruction::type() const
	{
		return static_cast<Type>(value >> 56 & 0xff);
	}

	std::uint64_t Instruction::arg1_56() const
	{
		return value & 0x00ffffffffffffff;
	}

	std::uint32_t Instruction::arg1_24() const
	{
		return static_cast<std::uint32_t>(value >> 32 & 0x00ffffff);
	}

	std::uint32_t Instruction::arg2_32() const
	{
		return static_cast<std::uint32_t>(value & 0xffffffff);
	}

	std::uint16_t Instruction::arg1_16() const
	{
		return static_cast<std::uint16_t>(value >> 32 & 0xffff);
	}

	std::uint16_t Instruction::arg2_16() const
	{
		return static_cast<std::uint16_t>(value >> 16 & 0xffff);
	}

	std::uint16_t Instruction::arg3_16() const
	{
		return static_cast<std::uint16_t>(value & 0xffff);
	}
}
