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

		struct Program
		{
			Constants constants;
			Functions functions;
			Bytecode bytecode;
		};

		class VM
		{
			public:
				VM() = default;
				~VM() = default;

				static Program loadBinary(const std::string& file);
				static void writeBinary(const Program& program, const std::string& file);

				void run(Program program, std::istream& in, std::ostream& out);

				void repl(std::istream& in, std::ostream& out);

				std::size_t callStackSize() const;

				static constexpr const char* BINARY_ID = ".svm";

			private:
				std::stack<StackFrame> callStack;
		};
	}
}

#endif
