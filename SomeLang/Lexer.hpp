#ifndef DBR_SVM_IL_LEXER_HPP
#define DBR_SVM_IL_LEXER_HPP

#include <vector>
#include <string>

#include "Token.hpp"

namespace dbr
{
	namespace sl
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
					
				TokenCode run(const std::string& input);
					
			private:
		};
	}
}

#endif
