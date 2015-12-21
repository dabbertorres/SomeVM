#include "Compiler.hpp"

#include <iostream>

namespace dbr
{
	namespace svm
	{
		namespace cpl
		{
			Compiler::Bytecode Compiler::run(const lex::Lexer::TokenCode& parsed)
			{
				Bytecode code;

				for(auto& p : parsed)
				{

				}

				return code;
			}
		}
	}
}
