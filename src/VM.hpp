#ifndef DBR_SVM_VM_HPP
#define DBR_SVM_VM_HPP

#include <stack>
#include <iostream>

#include "StackFrame.hpp"

namespace dbr
{
	namespace svm
	{
		namespace
		{
			constexpr unsigned int strLen(const char* str)
			{
				return str[0] != 0 ? strLen(str + 1) + 1 : 0;
			}
		}

		// number of arguments, StackFrame
		using Function = std::pair<std::size_t, StackFrame>;

		struct Program
		{
			Constants constants;
			std::vector<Function> functions;
		};

		class VM
		{
			public:
				VM(std::istream& in, std::ostream& out);
				~VM() = default;

				static Program loadBinary(const std::string& file);
				static void writeBinary(const Program& program, const std::string& file);

				void run(const Program& program);

				void repl();

				std::size_t callStackSize() const;

				static constexpr const char* BINARY_ID = ".svm";

			private:
				void interpret(const Program& program, StackFrame& currFrame, Bytecode::const_iterator& currInstr);
				
				std::stack<StackFrame> callStack;

				std::vector<Value> returns;

				std::istream& in;
				std::ostream& out;
		};
	}
}

#endif
