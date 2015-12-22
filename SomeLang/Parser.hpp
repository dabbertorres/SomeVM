#ifndef DBR_SVM_IL_PARSER_HPP
#define DBR_SVM_IL_PARSER_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

#include "Lexer.hpp"

namespace dbr
{
	namespace sl
	{
		class Parser
		{
			public:
				using VarMap = std::unordered_map<std::string, std::size_t>;

				Parser() = default;
				~Parser() = default;

				// maybe return something else. AST?
				// really should at least organize the raw TokenCode
				Lexer::TokenCode run(const Lexer::TokenCode&);

			private:
				VarMap variables;
		};
	}
}

#endif
