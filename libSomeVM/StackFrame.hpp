#pragma once

#include "libSomeVM.hpp"

#include "Instruction.hpp"
#include "Value.hpp"

namespace svm
{
	class Function;

	class SVM_API StackFrame
	{
	public:
		StackFrame(const Function& function, std::uint64_t functionIndex, std::uint64_t argsIdx);

		StackFrame(StackFrame&& other);
        StackFrame& operator=(StackFrame&& other);

		~StackFrame() = default;

		Bytecode::const_iterator next();
		bool complete() const;

		// absolute jump
		void jump(std::uint64_t instIdx);

		// relative jump
		void rjump(std::int64_t instOff);

		Bytecode::const_iterator begin() const;
		Bytecode::const_iterator end() const;

		std::uint64_t length() const;

		const Function& function;
		std::uint64_t functionIndex;

	private:
		std::uint64_t argsIdx;
		Bytecode::const_iterator currentInstruction;
	};
}
