#ifndef DBR_SVM_VM_STACK_FRAME_HPP
#define DBR_SVM_VM_STACK_FRAME_HPP

#include <stack>
#include <array>

#include "Instruction.hpp"
#include "Value.hpp"

namespace dbr
{
	namespace svm
	{
		using Bytecode = std::vector<Instruction>;
		using Constants = std::vector<Value>;

		// a function has a start index and a last index into the bytecode
		using Function = std::pair<std::size_t, std::size_t>;
		using Functions = std::vector<Function>;

		class StackFrame
		{
			public:
				static constexpr std::size_t REGISTRY_SIZE = 256;
				using Registry = std::array<Value, REGISTRY_SIZE>;

				using Return = std::pair<Registry::const_iterator, Registry::const_iterator>;

				StackFrame();
				StackFrame(const Bytecode& code);
				StackFrame(Registry::const_iterator begin, Registry::const_iterator end);
				StackFrame(const Bytecode& code, Registry::const_iterator begin, Registry::const_iterator end);
				~StackFrame() = default;

				// returns true when frame is finished (calling getReturn() becomes valid)
				// otherwise, returns false
				bool run(const Constants& constants, const Functions& functions);

				// returns valid iterators once run() returns true
				Return getReturn() const;

				void push(const Instruction& inst);

				void write(const Value& val);
				void write(const Value& val, std::size_t idx);
				Value read(std::size_t idx) const;

			private:
				void initRegistry(Registry::const_iterator begin, Registry::const_iterator end);

				Bytecode code;
				Bytecode::const_iterator currInst;
				Return returnPair;
				
				Registry registry;
				Registry::iterator nextEmpty;
		};
	}
}

#endif
