#ifndef DBR_SVM_STACK_FRAME_HPP
#define DBR_SVM_STACK_FRAME_HPP

#include <array>
#include <iostream>

#include "Instruction.hpp"
#include "Value.hpp"

namespace dbr
{
	namespace svm
	{
		class Function;

		class StackFrame
		{
		public:
			StackFrame(const Function& function, std::size_t argsIdx);

			StackFrame(const StackFrame& other);
			StackFrame(StackFrame&& other);

			~StackFrame() = default;

			Bytecode::const_iterator next();
			bool complete() const;

			// absolute jump
			void jump(Int instIdx);

			// relative jump
			void rjump(Int instOff);

			Bytecode::const_iterator begin() const;
			Bytecode::const_iterator end() const;

			std::size_t length() const;

			const Function& function;

		private:
			std::size_t argsIdx;
			Bytecode::const_iterator currentInstruction;
		};
	}
}

#endif
