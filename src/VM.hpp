#ifndef VM_HPP
#define VM_HPP

#include <stack>
#include <vector>
#include <istream>
#include <ostream>

#include "StackFrame.hpp"

namespace dbr
{
	namespace svm
	{
		class VM
		{
			public:
				VM() = default;
				~VM() = default;

				void run(const Bytecode& bytecode);

				void interpreter(std::istream& in, std::ostream& out);

				std::size_t stackSize() const;

			private:
				std::stack<StackFrame> stack;

				Constants constants;
				Functions functions;
		};
	}
}

#endif // VM_HPP
