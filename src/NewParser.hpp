#ifndef LNG_NEW_PARSER_HPP
#define LNG_NEW_PARSER_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

#include "Value.hpp"
#include "Instruction.hpp"

#include "Lexer.hpp"

namespace lng
{
	class NewParser
	{
		public:
			using Parsed = std::vector<std::string>;
			using VarMap = std::unordered_map<std::string, unsigned int>;

			NewParser() = default;
			~NewParser() = default;

			Parsed run(const std::string&);

		private:
			VarMap variables;
	};
}

#endif	// LNG_NEW_PARSER_HPP