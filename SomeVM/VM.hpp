#ifndef DBR_SVM_VM_HPP
#define DBR_SVM_VM_HPP

#include <stack>
#include <iostream>

#include "StackFrame.hpp"
#include "Program.hpp"

namespace dbr
{
	namespace svm
	{
		struct Breakpoint
		{
			std::size_t functionIndex;
			std::size_t instructionIndex;
		};

		class VM
		{
		public:
			VM(std::istream& inStream, std::ostream& outStream, std::size_t initialRegistrySize = 256);
			~VM() = default;

			void load(const Program& program);

			void run();

			void debug(const std::vector<Breakpoint>& breakpoints);

			std::size_t callStackSize() const;
			std::size_t registrySize() const;

			// implicit write - writes 'val' to the first unused register
			// if the registry is full, resizes it
			void write(Value val);

			// explicit write - writes 'val' to the provided register number 'idx'
			void write(std::size_t idx, Value val);

			Value read(std::size_t idx) const;

			void print(const Value& val) const;

		private:
			void interpret(Instruction instr, StackFrame& frame);

			std::stack<StackFrame> callStack;

			Registry registry;
			Registry::iterator nextFree;

			Registry constants;

			std::vector<Function> functions;

			std::istream& inStream;
			std::ostream& outStream;
		};
	}
}

#endif
