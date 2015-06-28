#ifndef LNG_PARSER_HPP
#define LNG_PARSER_HPP

#include <regex>
#include <map>
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

			Lexer();
			~Lexer() = default;

			TokenCode run(const Words&);
			
		private:
			std::vector<std::pair<Token::Type, std::regex>> tokenValues;
	};
}

#endif // LNG_PARSER_HPP
