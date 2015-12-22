#ifndef DBR_SVM_IL_COMPILER_HPP
#define DBR_SVM_IL_COMPILER_HPP

#include <vector>

#include "Parser.hpp"

namespace dbr
{
	namespace sl
	{
		class Compiler
		{
			public:
				using Bytecode = std::vector<byte>;

				Compiler() = default;

				Bytecode run(const Lexer::TokenCode&);
		};
	}
}

#endif	// DBR_SVM_IL_COMPILER_HPP