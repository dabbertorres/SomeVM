#ifndef DBR_SVM_FUNCTION_HPP
#define DBR_SVM_FUNCTION_HPP

#include "Instruction.hpp"

namespace dbr
{
	namespace svm
	{
		class Function
		{
			friend class Assembler;

		public:
			Function(std::uint8_t nrets, std::uint8_t nargs, Bytecode code);

			Function(const Function& other);
			Function(Function&& other);

			Function& operator=(const Function& other);
			Function& operator=(Function&& other);

			Bytecode::const_iterator begin() const;
			Bytecode::const_iterator end() const;

			std::uint32_t length() const;

			const Bytecode& bytecode() const;

			std::uint8_t returns() const;
			std::uint8_t args() const;

		private:
			std::uint8_t numReturns;
			std::uint8_t numArgs;
			Bytecode code;
		};
	}
}

#endif
