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
			constexpr std::size_t strLen(const char* str)
			{
				return str[0] != 0 ? strLen(str + 1) + 1 : 0;
			}
		}

		// number of returns, StackFrame, number of arguments
		struct Function
		{
			std::size_t numReturns;
			std::size_t numArgs;
			StackFrame frame;

			Function() = default;
			Function(std::size_t nrets, std::size_t nargs, StackFrame frame)
				: numReturns(nrets),
				numArgs(nargs),
				frame(frame)
			{}
		};

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
