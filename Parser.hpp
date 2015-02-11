#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <map>
#include <string>

#include "Value.hpp"
#include "Instruction.hpp"

namespace lng
{
	class Parser
	{
		public:
			using Bytecode = std::vector<byte>;

			Bytecode parseFile(const std::string& f, bool save);
			Bytecode parseLine(const std::string& l);

			class Operator
			{
				public:
					enum class Associativity
					{
						Left,
						Right
					};
					
					Operator(unsigned int prec, Associativity a);
					
					unsigned int precedence;
					Associativity associativity;
			};
			
		private:
			void parseExp(const std::string& e, Bytecode& bytecode);
			static bool isNumber(const std::string& str);
			
			std::map<std::string, unsigned int> variables;
			
			static const std::map<std::string, Operator> operators;
	};
}

#endif // PARSER_HPP
