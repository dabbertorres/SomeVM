#pragma once

#include <stack>

#include "Frame.hpp"

namespace svm
{
	struct Program;

	class VM
	{
	public:
		VM(std::uint64_t initialRegistrySize = 256);

        VM(VM&&) = default;
        VM& operator=(VM&&) = default;

        VM(const VM&) = delete;
        VM& operator=(const VM&) = delete;

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
		void interpret(Instruction instr, Frame& frame);

		std::stack<Frame> callStack;

		Registry registry;
		Registry::iterator nextFree;

		Registry constants;

		std::vector<Function> functions;
	};
}
