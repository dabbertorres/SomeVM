#include "Function.hpp"

namespace dbr
{
	namespace svm
	{
		Function::Function(std::uint8_t nrets, std::uint8_t nargs, Bytecode code)
		:	numReturns(nrets),
			numArgs(nargs),
			code(code)
		{}

		Function::Function(const Function& other)
		:	numReturns(other.numReturns),
			numArgs(other.numArgs),
			code(other.code)
		{}

		Function::Function(Function&& other)
		:	numReturns(other.numReturns),
			numArgs(other.numArgs),
			code(std::move(other.code))
		{}

		Function& Function::operator=(const Function& other)
		{
			numReturns = other.numReturns;
			numArgs = other.numArgs;
			code = other.code;
			
			return *this;
		}

		Function& Function::operator=(Function&& other)
		{
			numReturns = other.numReturns;
			numArgs = other.numArgs;
			code = std::move(other.code);

			return *this;
		}

		Bytecode::const_iterator Function::begin() const
		{
			return code.begin();
		}

		Bytecode::const_iterator Function::end() const
		{
			return code.end();
		}

		std::uint32_t Function::length() const
		{
			return code.size();
		}

		const Bytecode& Function::bytecode() const
		{
			return code;
		}

		std::uint8_t Function::returns() const
		{
			return numReturns;
		}

		std::uint8_t Function::args() const
		{
			return numArgs;
		}
	}
}
