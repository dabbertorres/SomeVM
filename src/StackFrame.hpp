#ifndef DBR_SVM_STACK_FRAME_HPP
#define DBR_SVM_STACK_FRAME_HPP

#include <array>
#include <iostream>

#include "Instruction.hpp"
#include "Value.hpp"

namespace dbr
{
	namespace svm
	{
		class StackFrame
		{
			public:
				using Registry = std::array<Value, 256>;

				using Return = std::pair<Registry::const_iterator, Registry::const_iterator>;

				StackFrame();
				StackFrame(const Bytecode& code);
				StackFrame(Registry::const_iterator begin, Registry::const_iterator end);
				StackFrame(const Bytecode& code, Registry::const_iterator begin, Registry::const_iterator end);

				StackFrame(const StackFrame& other);
				StackFrame(StackFrame&& other);

				StackFrame& operator=(const StackFrame& other);
				StackFrame& operator=(StackFrame&& other);

				~StackFrame() = default;

				// returns true when frame is finished (calling getReturn() becomes valid)
				// otherwise, returns false
				bool run(Constants& constants, const Functions& functions, std::istream& in, std::ostream& out);

				// returns valid Registry iterators once run() returns true
				Return getReturn() const;

				void push(const Instruction& inst);

				void write(std::size_t idx, const Value& val);
				Value read(std::size_t idx) const;

			private:
				Bytecode code;
				Bytecode::const_iterator currInst;
				Return returnPair;
				
				Registry registry;
		};
	}
}

#endif
