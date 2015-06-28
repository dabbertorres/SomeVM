#include "Parser.hpp"

#include <vector>
#include <stack>
#include <regex>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

namespace lng
{
	const std::map<std::string, Parser::Operator> Parser::operators = 
	{
		{"+", {6, Parser::Operator::Associativity::Left}}, {"-", {6, Parser::Operator::Associativity::Left}},
		{"*", {5, Parser::Operator::Associativity::Left}}, {"/", {5, Parser::Operator::Associativity::Left}},
		{"%", {5, Parser::Operator::Associativity::Left}}, {"=", {15, Parser::Operator::Associativity::Right}},
		{"==", {9, Parser::Operator::Associativity::Left}}, {"!=", {9, Parser::Operator::Associativity::Left}},
		{"<", {8, Parser::Operator::Associativity::Left}}, {">", {8, Parser::Operator::Associativity::Left}},
		{"<=", {8, Parser::Operator::Associativity::Left}}, {">=", {8, Parser::Operator::Associativity::Left}},
		{"()", {2, Parser::Operator::Associativity::Left}},
	};
	
	const std::map<std::string, std::function<void(Parser::Bytecode& bytecode)>> Parser::keywords = 
	{
		{"true", [](Bytecode& bytecode)
		{
			bytecode.push_back(static_cast<byte>(Instruction::PushLiteral));
			bytecode.push_back(static_cast<byte>(ValueType::Bool));
			bytecode.push_back(static_cast<byte>(true));
		}},
		{"false", [](Bytecode& bytecode)
		{
			bytecode.push_back(static_cast<byte>(Instruction::PushLiteral));
			bytecode.push_back(static_cast<byte>(ValueType::Bool));
			bytecode.push_back(static_cast<byte>(false));
		}},
	};
	
	const std::map<std::string, std::function<void(Parser::Bytecode& bytecode)>> Parser::functions = 
	{
		{"if", [](Bytecode& bytecode)
		{
			bytecode.push_back(static_cast<byte>(Instruction::If));
			bytecode.push_back(static_cast<byte>(bytecode.size() + 1));
			bytecode.push_back(static_cast<byte>(bytecode.size() + 1));
		}},
		{"while", [](Bytecode& bytecode)
		{
			
		}},
		{"include", [](Bytecode& bytecode)
		{
			
		}},
	};
														
	Parser::Bytecode Parser::parseFile(const std::string& f)
	{
		Bytecode instructions;
		
		std::ifstream fin(f);
		
		if(fin.bad())
			return instructions;
		
		std::vector<std::string> lines(1);
		variables.clear();
		
		while(std::getline(fin, lines.back()))
		{
			if(!lines.back().empty() && lines.back().substr(0, 2) != "//")
			{
				auto toRemove = std::remove_if(lines.back().begin(), lines.back().end(), [](const char c)
				{
					return c == ' ' || c == '\n' || c == '\t';
				});
				lines.back().erase(toRemove, lines.back().end());
				
				lines.emplace_back();
			}
		}
		
		// remove empty last line
		lines.erase(lines.end() - 1);
		
		for(auto& l : lines)
		{
			std::cout << "line: " << l << '\n';
		}
		
		for(auto& l : lines)
		{
			parseExp(l, instructions);
		}
		
		instructions.push_back(static_cast<byte>(lng::Instruction::End));
		
		std::ofstream fout(f + ".run", std::ios::binary);
		
		for(auto& i : instructions)
			fout << i;
		
		return instructions;
	}
	
	Parser::Bytecode Parser::parseLine(const std::string& l)
	{
		Bytecode instructions;
		
		parseExp(l, instructions);

		return instructions;
	}
	
	Parser::Operator::Operator(unsigned int prec, Operator::Associativity a)
	:	precedence(prec),
		associativity(a)
	{}
	
	void Parser::parseExp(const std::string& e, Bytecode& bytecode)
	{
		std::vector<std::string> tokens(1);
		
		// lexer/tokenizer
		for(auto it = e.begin(); it != e.end(); it++)
		{
			if(std::isdigit(*it) || *it == '.')
			{
				if(std::isdigit(tokens.back().back()) || tokens.back().back() == '.' || tokens.back().empty())
					tokens.back().push_back(*it);
				else
					tokens.emplace_back(std::string(1, *it));
			}
			else if(std::isalpha(*it))	// is variable/keyword/function
			{
				if(std::isalpha(tokens.back().back()) || tokens.back().empty())
					tokens.back().push_back(*it);
				else
					tokens.emplace_back(std::string(1, *it));
			}
			else	// operators
			{
				std::string multiCharOp;
				multiCharOp += *it;
				multiCharOp += *(it + 1);
				
				if(operators.find(multiCharOp) != operators.end())
				{
					tokens.emplace_back(multiCharOp);
					it++;
				}
				else if((operators.find(std::string(1, *it)) != operators.end()) || *it == '(' || *it == ')')
				{
					if(tokens.back().empty())
					{
						tokens.back().push_back(*it);
						tokens.emplace_back();
					}
					else
					{
						tokens.emplace_back();
						tokens.back().push_back(*it);
					}
				}
			}
		}
		
		std::string output;
		std::stack<std::string> operations;
		
		// Shunting-yard algorithm for parsing to Reverse-Polish Notation
		for(auto& t : tokens)
		{
			std::cout << "token: " << t << '\n';
			if(isNumber(t))
			{
				output.append(t);
				output.push_back(' ');
			}
			else if(t == "(")
			{
				operations.push(t);
			}
			else if(t == ")")
			{
				while(operations.top() != "(")
				{
					output.append(operations.top());
					output.push_back(' ');
					operations.pop();
				}
				
				operations.pop();
			}
			else if(operators.find(t) != operators.end())
			{
				const Operator o1 = operators.at(t);
				Operator o2(0, Operator::Associativity::Left);
				if(!operations.empty() && operations.top() != "(")
				{
					if(functions.find(operations.top()) != functions.end())
						o2 = operators.at("()");
					else
						o2 = operators.at(operations.top());
				}
				
				while(!operations.empty() && operations.top() != "(" &&
					((o1.associativity == Operator::Associativity::Left && o1.precedence >= o2.precedence) || 
					(o1.associativity == Operator::Associativity::Right && o1.precedence > o2.precedence)))
				{
					output.append(operations.top());
					output.push_back(' ');
					operations.pop();
					
					if(!operations.empty())
					{
						if(functions.find(operations.top()) != functions.end())
							o2 = operators.at("()");
						else
							o2 = operators.at(operations.top());
					}
				}
					
				operations.push(t);
			}
			else if(functions.find(t) != functions.end())
			{
				operations.push(t);
			}
			else	// is variable
			{
				output.append(t);
				output.push_back(' ');
				variables.emplace(t, variables.size());
			}
		}
		
		while(!operations.empty())
		{
			for(auto& c : operations.top())
				output.push_back(c);
			output.push_back(' ');
			operations.pop();
		}
		
		// empty the tokens vector for reuse
		// and divvy up the output to single strings
		tokens.clear();
		std::stringstream ss(output);
		std::string token;
		
		while(std::getline(ss, token, ' '))
			tokens.push_back(token);
		
		// compile RPN to bytecode
		std::string var = "";
		for(auto it = tokens.begin(); it != tokens.end(); it++)
		{
			if(isNumber(*it))
			{
				bytecode.push_back(static_cast<byte>(Instruction::PushLiteral));
				bytecode.push_back(static_cast<byte>(ValueType::Number));
				Float f;
				std::cout << "stof: " << *it << '\n';
				f.value = std::stof(*it);
				bytecode.push_back(static_cast<byte>(f.bytes[0]));
				bytecode.push_back(static_cast<byte>(f.bytes[1]));
				bytecode.push_back(static_cast<byte>(f.bytes[2]));
				bytecode.push_back(static_cast<byte>(f.bytes[3]));
			}
			else if(operators.find(*it) != operators.end())
			{
				if(*it == "+")
				{
					bytecode.push_back(static_cast<byte>(Instruction::Add));
				}
				else if(*it == "-")
				{
					bytecode.push_back(static_cast<byte>(Instruction::Sub));
				}
				else if(*it == "*")
				{
					bytecode.push_back(static_cast<byte>(Instruction::Mult));
				}
				else if(*it == "/")
				{
					bytecode.push_back(static_cast<byte>(Instruction::Div));
				}
				else if(*it == "%")
				{
					bytecode.push_back(static_cast<byte>(Instruction::Mod));
				}
				else if(*it == "=")
				{
					bytecode.push_back(static_cast<byte>(Instruction::Store));
					
					if(variables.find(var) != variables.end())
					{
						bytecode.push_back(static_cast<byte>(variables.at(var)));
					}
					else
					{
						bytecode.push_back(static_cast<byte>(variables.size()));
					}
				}
				else if(*it == "==")
				{
					bytecode.push_back(static_cast<byte>(Instruction::Equal));
				}
				else if(*it == "!=")
				{
					bytecode.push_back(static_cast<byte>(Instruction::NotEqual));
				}
				else if(*it == "<")
				{
					bytecode.push_back(static_cast<byte>(Instruction::Lesser));
				}
				else if(*it == ">")
				{
					bytecode.push_back(static_cast<byte>(Instruction::Greater));
				}
				else if(*it == "<=")
				{
					bytecode.push_back(static_cast<byte>(Instruction::LesserOrEqual));
				}
				else if(*it == ">=")
				{
					bytecode.push_back(static_cast<byte>(Instruction::GreaterOrEqual));
				}
			}
			else if(keywords.find(*it) != keywords.end())
			{
				keywords.at(*it)(bytecode);
			}
			else if(functions.find(*it) != functions.end())
			{
				functions.at(*it)(bytecode);
			}
			else	// is variable
			{
				// if the first token is this variable, then we're assigning, not reading
				if(it != tokens.begin())
				{
					bytecode.push_back(static_cast<byte>(Instruction::PushVariable));
					bytecode.push_back(static_cast<byte>(variables.at(*it)));
				}
				else
					var = *it;
			}
		}
	}
	
	bool Parser::isNumber(const std::string& str)
	{
		for(auto& c : str)
		{
			std::cout << "char: " << c << '\n';
			if(!std::isdigit(c) && c != '.')
				return false;
		}
		std::cout << "isNumber: " << str << '\n';
		return true;
	}
}
