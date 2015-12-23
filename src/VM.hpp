#ifndef DBR_SVM_VM_HPP
#define DBR_SVM_VM_HPP

#include <stack>
#include <iostream>

#include "StackFrame.hpp"

namespace dbr
{
	namespace svm
	{
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

				void run(Program program, std::istream& in, std::ostream& out);

				void interpreter(std::istream& in, std::ostream& out);

				std::size_t callStackSize() const;

			private:
				std::stack<StackFrame> callStack;
		};
	}
}

#endif
