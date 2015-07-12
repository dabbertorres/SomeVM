#include "Lexer.hpp"

#include <stdexcept>

namespace lng
{
	Lexer::TokenCode Lexer::run(const std::string& input)
	{
		TokenCode tokens;
		
		// I probably need to go character by character in a string first. If I find a Token::Type::Symbol*,
		// I should split the string there. I could maybe even "pre-process" it. Before this for loop, iterate
		// over the input, and modify and insert strings into input as needed to do this.
		for(auto i = 0u; i < input.size(); ++i)
		{
			auto nextWS = input.find_first_of(" \n\t", i);
			std::string sub = input.substr(i, nextWS - i);
			
			if(sub.empty())
				continue;
			
			bool matchFound = false;
			
			for(auto& p : tokenValues)
			{
				if(std::regex_match(sub, p.second))
				{
					if(p.first == Token::Type::LineComment)
					{
						i = input.find_first_of('\n', i);
					}
					else if(p.first == Token::Type::BlockComment)
					{
						i = input.find("*/", i);
					}
					else
					{
						i += sub.length();
					}
					
					tokens.emplace_back(p.first, sub);
					
					matchFound = true;
					break;
				}
			}
			
			if(!matchFound)
			{
				throw std::runtime_error("Unknown symbol: \"" + sub + "\"\n");
			}
		}
		
		return tokens;
	}
	
	#define RGX(s) std::regex(s/*, std::regex::extended*/)
	
	const std::vector<std::pair<Token::Type, std::regex>> Lexer::tokenValues = 
	{
		{Token::Type::ValueInteger, RGX("[+-]?[0-9]+")},
		{Token::Type::ValueFloat, RGX("[+-]?[0-9]+[.]?[0-9f]+")},
		{Token::Type::ValueChar, RGX("\'./s\'")},
		{Token::Type::ValueString, RGX("\"./s*\"")},
		{Token::Type::ValueBool, RGX("true|false")},
		
		{Token::Type::TypeInteger, RGX("int")},
		{Token::Type::TypeFloat, RGX("float")},
		{Token::Type::TypeChar, RGX("char")},
		{Token::Type::TypeString, RGX("string")},
		{Token::Type::TypeBool, RGX("bool")},
		{Token::Type::TypeObject, RGX("class")},
		{Token::Type::TypeFunction, RGX("function")},
		{Token::Type::TypeArray, RGX("\\[[0-9]*\\]")},
		
		{Token::Type::Identifier, RGX("[a-zA-Z]+")},
		
		{Token::Type::MemoryAllocate, RGX("alloc")},
		{Token::Type::MemoryDeallocate, RGX("dealloc")},
		
		{Token::Type::Pointer, RGX("@+")},
		
		{Token::Type::OperatorAdd, RGX("\\+")},
		{Token::Type::OperatorSub, RGX("-")},
		{Token::Type::OperatorMult, RGX("\\*")},
		{Token::Type::OperatorDiv, RGX("\\/")},
		{Token::Type::OperatorMod, RGX("%")},
		
		{Token::Type::OperatorBitwiseAnd, RGX("&")},
		{Token::Type::OperatorBitwiseOr, RGX("\\|")},
		{Token::Type::OperatorBitwiseXor, RGX("\\^")},
		{Token::Type::OperatorBitwiseNot, RGX("~")},
		
		{Token::Type::OperatorShiftLeft, RGX("<<")},
		{Token::Type::OperatorShiftRight, RGX(">>")},
		
		{Token::Type::OperatorEqual, RGX("==")},
		{Token::Type::OperatorNotEqual, RGX("!=")},
		{Token::Type::OperatorLesser, RGX("<")},
		{Token::Type::OperatorGreater, RGX(">")},
		{Token::Type::OperatorLesserEqual, RGX("<=")},
		{Token::Type::OperatorGreaterEqual, RGX(">=")},
		
		{Token::Type::OperatorAnd, RGX("&&")},
		{Token::Type::OperatorOr, RGX("\\|\\|")},
		{Token::Type::OperatorXor, RGX("\\^\\^")},
		{Token::Type::OperatorNot, RGX("!")},
		
		{Token::Type::OperatorAssign, RGX("=")},
		{Token::Type::OperatorAddAssign, RGX("\\+=")},
		{Token::Type::OperatorSubAssign, RGX("-=")},
		{Token::Type::OperatorMultAssign, RGX("\\*=")},
		{Token::Type::OperatorDivAssign, RGX("\\/=")},
		{Token::Type::OperatorModAssign, RGX("%=")},
		{Token::Type::OperatorBitwiseAndAssign, RGX("&=")},
		{Token::Type::OperatorBitwiseOrAssign, RGX("\\|=")},
		{Token::Type::OperatorBitwiseXorAssign, RGX("\\^=")},
		{Token::Type::OperatorBitwiseNotAssign, RGX("~=")},
		{Token::Type::OperatorShiftLeftAssign, RGX("<<=")},
		{Token::Type::OperatorShiftRightAssign, RGX(">>=")},
		
		{Token::Type::Increment, RGX("\\+\\+")},
		{Token::Type::Decrement, RGX("--")},
		
		{Token::Type::OperatorAddressOf, RGX("\\?+")},
		{Token::Type::OperatorDereference, RGX("@+")},
		
		{Token::Type::LineComment, RGX("\\s*\\/\\/")},
		{Token::Type::BlockComment, RGX("\\/\\*")},
		
		{Token::Type::SymbolOpenParen, RGX("\\(")},
		{Token::Type::SymbolEndParen, RGX("\\)")},
		{Token::Type::SymbolOpenSquareBracket, RGX("\\[")},
		{Token::Type::SymbolEndSquareBracket, RGX("\\]")},
		{Token::Type::SymbolOpenCurlyBrace, RGX("\\{")},
		{Token::Type::SymbolEndCurlyBrace, RGX("\\}")},
		{Token::Type::SymbolOpenAngleBracket, RGX("<")},
		{Token::Type::SymbolEndAngleBracket, RGX(">")},
		{Token::Type::SymbolSingleQuote, RGX("\'")},
		{Token::Type::SymbolDoubleQuote, RGX("\"")},
	};
	
	#undef RGX
}
