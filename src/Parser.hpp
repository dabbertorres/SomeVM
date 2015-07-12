#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "Lexer.hpp"

#include "Value.hpp"
#include "Instruction.hpp"

namespace lng
{
	class Parser
	{
		public:
			using Bytecode = std::vector<byte>;

			Bytecode parse(const Lexer::TokenCode&);

			class Operator
			{
				public:
					enum class Associativity
					{
						Left,
						Right
					};
					
					Operator(unsigned int, Associativity);
					
					unsigned int precedence;
					Associativity associativity;
			};
			
		private:
			void parseExp(const std::string&, Bytecode&);
			static bool isNumber(const std::string&);
			
			std::map<std::string, unsigned int> variables;
			
			static const std::map<std::string, Operator> operators;
			static const std::map<std::string, std::function<void(Bytecode&)>> keywords;
			static const std::map<std::string, std::function<void(Bytecode&)>> functions;
	};
}

#endif // PARSER_HPP
