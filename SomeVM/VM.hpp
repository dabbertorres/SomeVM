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
		class VM
		{
		public:
			VM(std::istream& in, std::ostream& out, std::size_t initialRegistrySize = 256);
			~VM() = default;

			void load(const Program& program);

			void run();

			std::size_t callStackSize() const;
			std::size_t registrySize() const;

			// implicit write - writes 'val' to the first unused register
			// if the registry is full, resizes it
			void write(Value val);

			// explicit write - writes 'val' to the provided register number 'idx'
			void write(std::size_t idx, Value val);

			Value read(std::size_t idx) const;

		private:
			void interpret();

			std::stack<StackFrame> callStack;

			Registry registry;
			Registry::iterator nextFree;

			Registry constants;

			std::vector<Function> functions;

			std::istream& in;
			std::ostream& out;
		};
	}
}

#endif
