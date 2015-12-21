#ifndef DBR_SVM_LEX_LEXER_HPP
#define DBR_SVM_LEX_LEXER_HPP

#include <vector>
#include <string>

#include "Token.hpp"

namespace dbr
{
	namespace svm
	{
		namespace lex
		{
			class Lexer
			{
				public:
					using TokenCode = std::vector<Token>;
					
					enum class State
					{
						
					};
					
					Lexer() = default;
					~Lexer() = default;
					
					TokenCode run(const std::string&);
					
				private:
			};
		}
	}
}

#endif
