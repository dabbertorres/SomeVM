#pragma once

#include <cstdint>

#include "Instruction.hpp"

namespace svm
{
	class Function
	{
	public:
		Function(uint8_t nrets, uint8_t nargs, Bytecode code);

		Function(const Function& other);
		Function(Function&& other);

		Function& operator=(const Function& other);
		Function& operator=(Function&& other);

		Bytecode::const_iterator begin() const;
		Bytecode::const_iterator end() const;

		uint64_t length() const;

		const Bytecode& bytecode() const;

		uint8_t returns() const;
		uint8_t args() const;

	private:
		uint8_t nrets;
		uint8_t nargs;
		Bytecode code;
	};
}
