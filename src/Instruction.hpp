#ifndef DBR_SVM_INSTRUCTIONS_HPP
#define DBR_SVM_INSTRUCTIONS_HPP

#include <cstdint>
#include <vector>

namespace dbr
{
	namespace svm
	{
		/*
			An instruction is encoded in 4 bytes.

			byte 0:	instruction type (may be shrunk in the future. Not even coming close to using the whole byte)
			byte 1: arg 1 - usually the registry write-to index
			byte 2: arg 2
			byte 3: arg 3

			bytes 1, 2, & 3 can be used as "arg 1x", returning a uint (32 bits), with a max of 24 bits being used

			bytes 2 & 3 can be used as "arg 2x", returning a ushort (16 bits), rather than a byte
			usually used to index into the constants table
		 */
		class Instruction
		{
			public:
				enum class Type : std::uint8_t
				{
					// memory ops
					Load,		// 1: write-to, 2: index
					LoadConst,	// 1: write-to, 2x: constant index

					// math ops
					Add,		// "addition" 1: write-to, 2: registry index, 3: registry index
					Sub,		// "subtraction" 1: write-to, 2: registry index, 3: registry index
					Mult,		// "multiplication" 1: write-to, 2: registry index, 3: registry index
					Div,		// "division" 1: write-to, 2: registry index, 3: registry index
					Mod,		// "modulus" 1: write-to, 2: registry index, 3: registry index
					Neg,		// "negative" 1: write-to, 2: registry index

					// comparison
					Not,		// 1: write-to, 2: registry index
					Less,		// "less than" 1: write-to, 2: registry index, 3: registry index
					LessEq,		// "less than or equal" 1: write-to, 2: registry index, 3: registry index
					Gret,		// "greater than" 1: write-to, 2: registry index, 3: registry index
					GretEq,		// "greater than or equal" 1: write-to, 2: registry index, 3: registry index
					Eq,			// "equals" 1: write-to, 2: registry index, 3: registry index
					Neq,		// "not equals" 1: write-to, 2: registry index, 3: registry index

					// conditions
					If,			// 1: registry index
					// next instruction is a jump, executed if argument is false (the "else")
					// if argument is true, jump is skipped (the "then"). If an "else" exists, the end of the "then" will have a jump

					// branching
					Call,		// 1x: function index
					Ret,		// "return" 1: starting registry index to return values from, 2x: ending registry index to return values from. If 2x is greater than 255, nothing is returned
					Jump,		// 1x: instruction index (can only jump within current stack frame)

					// misc
					Noop,		// "No operation". Any arguments are ignored
					Print,		// 1: registry index to print the value of
				};

				Instruction();
				Instruction(std::uint32_t val);
				Instruction(Type t, std::uint32_t);
				Instruction(Type t, std::uint8_t, std::uint16_t);
				Instruction(Type t, std::uint8_t, std::uint8_t, std::uint8_t);

				Type type() const;
				std::uint8_t arg1() const;
				std::uint32_t arg1x() const;
				std::uint8_t arg2() const;
				std::uint16_t arg2x() const;
				std::uint8_t arg3() const;

			private:
				std::uint32_t value;
		};

		using Bytecode = std::vector<Instruction>;

		// a function has a start index and a last index into the bytecode
		using Function = std::pair<std::size_t, std::size_t>;
		using Functions = std::vector<Function>;
	}
}

#endif
