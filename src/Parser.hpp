#ifndef DBR_SVM_PRS_PARSER_HPP
#define DBR_SVM_PRS_PARSER_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

#include "Value.hpp"
#include "Instruction.hpp"

#include "Lexer.hpp"

namespace dbr
{
	namespace svm
	{
		namespace prs
		{
			class Parser
			{
				public:
					using VarMap = std::unordered_map<std::string, unsigned int>;

					Parser() = default;
					~Parser() = default;

					// maybe return something else. AST?
					// really should at least organize the raw TokenCode
					lex::Lexer::TokenCode run(const lex::Lexer::TokenCode&);

				private:
					VarMap variables;
			};
		}
	}
}

#endif
