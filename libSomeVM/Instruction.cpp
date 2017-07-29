#include "Instruction.hpp"

namespace svm
{
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
