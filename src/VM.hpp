#ifndef VM_HPP
#define VM_HPP

#include <stack>
#include <vector>
#include <unordered_map>

#include "Parser.hpp"

#include "Value.hpp"

#include "Instruction.hpp"

namespace lng
{
	class VM
	{
		public:
			using Bytecode = std::vector<byte>;
			
			VM(unsigned int ss);
			~VM();

			void run(const Bytecode& bytecode);
			
			static Bytecode loadBytes(const std::string& f);
			
			unsigned int getStackSize() const;
			
		private:
			std::stack<const BaseValue*> stack;
			std::vector<BaseValue*> variables;
			const unsigned int MAX_STACK_SIZE;
			
			Bytecode::const_iterator previous;
	};
}

#endif // VM_HPP
