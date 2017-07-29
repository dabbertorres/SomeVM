#pragma once

#include "libSomeVM.hpp"

#include <stack>

#include "StackFrame.hpp"

namespace svm
{
	struct Program;

	using Registry = std::vector<Value>;

	class SVM_API VM
	{
	public:
		VM(std::uint64_t initialRegistrySize = 256);
		~VM() = default;

		void load(const Program& program);

		void run();

		std::uint64_t callStackSize() const;
		std::uint64_t registrySize() const;

		// implicit write - writes 'val' to the first unused register
		// if the registry is full, resizes it
		void write(Value val);

		// explicit write - writes 'val' to the provided register number 'idx'
		void write(std::uint64_t idx, Value val);

		Value read(std::uint64_t idx) const;

	private:
		void interpret(Instruction instr, StackFrame& frame);

		std::stack<StackFrame> callStack;

		Registry registry;
		Registry::iterator nextFree;

		Registry constants;

		std::vector<Function> functions;
	};
}
