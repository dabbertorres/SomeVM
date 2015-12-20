#ifndef LNG_LEXER_HPP
#define LNG_LEXER_HPP

#include <vector>
#include <string>

#include "Token.hpp"

namespace lng
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

#endif
