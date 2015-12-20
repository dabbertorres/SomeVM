#include "NewParser.hpp"

#include <stack>
#include <fstream>
#include <iostream>

namespace lng
{
	NewParser::Parsed NewParser::run(const std::string& file)
	{
		std::ifstream fin(file);

		if(!fin)
			return {};
		
		std::vector<std::string> expressions(1);

		while(std::getline(fin, expressions.back(), ';'))
			expressions.emplace_back();

		expressions.pop_back();	// get rid of last empty exp

		Parsed parsed;
		for(auto& e : expressions)
		{
			std::cout << "Expression: " << e << '\n';
			Lexer lexer;

			Lexer::TokenCode tokenCode = lexer.run(e);

			std::cout << "\tTokens:\n";
			for(auto& t : tokenCode)
				std::cout << "\t\t" << t.value << '\n';
		}

		return parsed;
	}
}
