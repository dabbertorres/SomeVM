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
			using Registry = std::vector<Value>;

			StackFrame();
			StackFrame(Bytecode code);
			StackFrame(Bytecode code, Registry::const_iterator begin, Registry::const_iterator end);

			StackFrame(const StackFrame& other);
			StackFrame(StackFrame&& other);

			StackFrame& operator=(const StackFrame& other);
			StackFrame& operator=(StackFrame&& other);

			~StackFrame() = default;

			Bytecode::const_iterator next();
			void jump(Int instIdx);

			void push(const Instruction& inst);

			// implicit write - writes 'val' to the first unused register
			void write(Value val);

			// explicit write - writes 'val' to the provided register number 'idx'
			void write(std::size_t idx, Value val);

			// implicit write - writes all Values from 'begin'..'end' starting at the first unused register
			void writeAll(Registry::const_iterator begin, Registry::const_iterator end);

			// explicit write - writes all Values from 'begin'..'end' starting at the provided register number 'idx'
			void writeAll(std::size_t idx, Registry::const_iterator begin, Registry::const_iterator end);

			Value read(std::size_t idx) const;

			const Registry& getRegistry() const;
			const Bytecode& getBytecode() const;

			Bytecode::const_iterator codeBegin() const;
			Bytecode::const_iterator codeEnd() const;

			Registry::const_iterator regBegin() const;
			Registry::const_iterator regEnd() const;

		private:
			static constexpr std::size_t registrySize = 256;

			Bytecode code;
			Bytecode::const_iterator currInstr;

			Registry registry;
			Registry::iterator nextFree;
		};
	}
}

#endif
