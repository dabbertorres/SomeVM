#include "Instruction.hpp"

namespace dbr
{
	namespace svm
	{
		Instruction::Instruction()
			: Instruction(Type::Nop, 0)
		{}

		Instruction::Instruction(std::uint32_t val)
			: value(val)
		{}

		Instruction::Instruction(Type t, std::uint32_t oneX)
			: value(0)
		{
			value = (static_cast<std::uint8_t>(t) << 24 & 0xFF000000) | (oneX & 0xFFFFFF);
		}

		Instruction::Instruction(Type t, std::uint8_t one, std::uint16_t twoX)
			: value(0)
		{
			value = (static_cast<std::uint8_t>(t) << 24 & 0xFF000000) | (one << 16 & 0xFF0000) | twoX;
		}

		Instruction::Instruction(Type t, std::uint8_t one, std::uint8_t two, std::uint8_t three)
			: value(0)
		{
			value = (static_cast<std::uint8_t>(t) << 24 & 0xFF000000) | (one << 16 & 0xFF0000) | (two << 8 & 0xFF00) | three;
		}

		Instruction::Type Instruction::type() const
		{
			return static_cast<Type>(value >> 24 & 0xFF);
		}

		std::uint8_t Instruction::arg1() const
		{
			return static_cast<std::uint8_t>(value >> 16 & 0xFF);
		}

		std::uint32_t Instruction::arg1x() const
		{
			return static_cast<std::uint32_t>(value & 0xFFFFFF);
		}

		std::int32_t Instruction::arg1xs() const
		{
			return value & 0xFFFFFF;
		}

		std::uint8_t Instruction::arg2() const
		{
			return static_cast<std::uint8_t>(value >> 8 & 0xFF);
		}

		std::uint16_t Instruction::arg2x() const
		{
			return static_cast<std::uint16_t>(value & 0xFFFF);
		}

		std::uint8_t Instruction::arg3() const
		{
			return static_cast<std::uint8_t>(value & 0xFF);
		}
	}
}
