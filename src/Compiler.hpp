#ifndef LNG_COMPILER_HPP
#define LNG_COMPILER_HPP

#include <vector>

#include "Instruction.hpp"
#include "Parser.hpp"

namespace dbr
{
	namespace svm
	{
		namespace cpl
		{
			class Compiler
			{
				public:
					using Bytecode = std::vector<byte>;

					Compiler() = default;

					Bytecode run(const lex::Lexer::TokenCode&);
			};
		}
	}
}

#endif	// LNG_COMPILER_HPP