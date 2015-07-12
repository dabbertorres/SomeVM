#ifndef LNG_PARSER_HPP
#define LNG_PARSER_HPP

#include <regex>
#include <vector>
#include <string>

#include "Token.hpp"

namespace lng
{
	class Lexer
	{
		public:
			using TokenCode = std::vector<Token>;
			using Words = std::vector<std::string>;
			
			Lexer() = default;
			~Lexer() = default;
			
			TokenCode run(const std::string&);
			
		private:
			const static std::vector<std::pair<Token::Type, std::regex>> tokenValues;
	};
}

#endif // LNG_PARSER_HPP
