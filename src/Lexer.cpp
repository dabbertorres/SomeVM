#include "Lexer.hpp"

#include <stdexcept>

namespace lng
{
	Lexer::TokenCode Lexer::run(const Words& input)
	{
		TokenCode tokens;
		
		bool inComment = false;
		
		for(auto& w : input)
		{
			if(inComment)
			{
				if(w.find('\n') != std::string::npos)
				{
					inComment = false;
				}
				
				continue;
			}
			
			bool matchFound = false;
			
			for(auto& p : tokenValues)
			{
				if(std::regex_match(w, p.second))
				{
					if(p.first == Token::Type::LineComment)
					{
						inComment = true;
					}
					else
					{
						tokens.emplace_back(p.first, w);
					}
					
					matchFound = true;
					break;
				}
			}
			
			if(!matchFound)
			{
				throw std::runtime_error("Unknown symbol: \"" + w + "\"\n");
			}
		}
		
		return tokens;
	}
	
	#define REGEX(s) std::regex(s/*, std::regex::extended*/)
	
	Lexer::Lexer()
	:	tokenValues({
		{Token::Type::ValueInteger, REGEX("[+-]?[0-9]+")},
		{Token::Type::ValueFloat, REGEX("[+-]?[0-9]+[.]?[0-9f]+")},
		{Token::Type::ValueChar, REGEX("\'./s\'")},
		{Token::Type::ValueString, REGEX("\"./s*\"")},
		{Token::Type::ValueBool, REGEX("true|false")},
		
		{Token::Type::TypeInteger, REGEX("int")},
		{Token::Type::TypeFloat, REGEX("float")},
		{Token::Type::TypeChar, REGEX("char")},
		{Token::Type::TypeString, REGEX("string")},
		{Token::Type::TypeBool, REGEX("bool")},
		{Token::Type::TypeObject, REGEX("class")},
		{Token::Type::TypeFunction, REGEX("function")},
		{Token::Type::TypeArray, REGEX("\\[[0-9]*\\]")},
		
		{Token::Type::Identifier, REGEX("[a-zA-Z]+")},
		
		{Token::Type::MemoryAllocate, REGEX("alloc")},
		{Token::Type::MemoryDeallocate, REGEX("dealloc")},
		
		{Token::Type::Pointer, REGEX("@+")},
		
		{Token::Type::OperatorAdd, REGEX("\\+")},
		{Token::Type::OperatorSub, REGEX("-")},
		{Token::Type::OperatorMult, REGEX("\\*")},
		{Token::Type::OperatorDiv, REGEX("\\/")},
		{Token::Type::OperatorMod, REGEX("%")},
		
		{Token::Type::OperatorBitwiseAnd, REGEX("&")},
		{Token::Type::OperatorBitwiseOr, REGEX("\\|")},
		{Token::Type::OperatorBitwiseXor, REGEX("\\^")},
		{Token::Type::OperatorBitwiseNot, REGEX("~")},
		
		{Token::Type::OperatorShiftLeft, REGEX("<<")},
		{Token::Type::OperatorShiftRight, REGEX(">>")},
		
		{Token::Type::OperatorEqual, REGEX("==")},
		{Token::Type::OperatorNotEqual, REGEX("!=")},
		{Token::Type::OperatorLesser, REGEX("<")},
		{Token::Type::OperatorGreater, REGEX(">")},
		{Token::Type::OperatorLesserEqual, REGEX("<=")},
		{Token::Type::OperatorGreaterEqual, REGEX(">=")},
		
		{Token::Type::OperatorAnd, REGEX("&&")},
		{Token::Type::OperatorOr, REGEX("\\|\\|")},
		{Token::Type::OperatorXor, REGEX("\\^\\^")},
		{Token::Type::OperatorNot, REGEX("!")},
		
		{Token::Type::OperatorAssign, REGEX("=")},
		{Token::Type::OperatorAddAssign, REGEX("\\+=")},
		{Token::Type::OperatorSubAssign, REGEX("-=")},
		{Token::Type::OperatorMultAssign, REGEX("\\*=")},
		{Token::Type::OperatorDivAssign, REGEX("\\/=")},
		{Token::Type::OperatorModAssign, REGEX("%=")},
		{Token::Type::OperatorBitwiseAndAssign, REGEX("&=")},
		{Token::Type::OperatorBitwiseOrAssign, REGEX("\\|=")},
		{Token::Type::OperatorBitwiseXorAssign, REGEX("\\^=")},
		{Token::Type::OperatorBitwiseNotAssign, REGEX("~=")},
		{Token::Type::OperatorShiftLeftAssign, REGEX("<<=")},
		{Token::Type::OperatorShiftRightAssign, REGEX(">>=")},
		
		{Token::Type::Increment, REGEX("\\+\\+")},
		{Token::Type::Decrement, REGEX("--")},
		
		{Token::Type::OperatorAddressOf, REGEX("\\?+")},
		{Token::Type::OperatorDereference, REGEX("@+")},
		
		{Token::Type::LineComment, REGEX("\\/\\/")}
	})
	{}
	
	/*const std::vector<std::pair<Token::Type, std::regex>> Lexer::tokenValues = 
	{
		{Token::Type::ValueInteger, REGEX("[+-]?[0-9]+")},
		{Token::Type::ValueFloat, REGEX("[+-]?[0-9]+[.]?[0-9f]+")},
		{Token::Type::ValueChar, REGEX("\'./s\'")},
		{Token::Type::ValueString, REGEX("\"./s*\"")},
		{Token::Type::ValueBool, REGEX("true|false")},
		
		{Token::Type::TypeInteger, REGEX("int")},
		{Token::Type::TypeFloat, REGEX("float")},
		{Token::Type::TypeChar, REGEX("char")},
		{Token::Type::TypeString, REGEX("string")},
		{Token::Type::TypeBool, REGEX("bool")},
		{Token::Type::TypeObject, REGEX("class")},
		{Token::Type::TypeFunction, REGEX("function")},
		{Token::Type::TypeArray, REGEX("\\[[0-9]*\\]")},
		
		{Token::Type::Identifier, REGEX("[a-zA-Z]+")},
		
		{Token::Type::MemoryAllocate, REGEX("alloc")},
		{Token::Type::MemoryDeallocate, REGEX("dealloc")},
		
		{Token::Type::Pointer, REGEX("@+")},
		
		{Token::Type::OperatorAdd, REGEX("\\+")},
		{Token::Type::OperatorSub, REGEX("-")},
		{Token::Type::OperatorMult, REGEX("\\*")},
		{Token::Type::OperatorDiv, REGEX("\\/")},
		{Token::Type::OperatorMod, REGEX("%")},
		
		{Token::Type::OperatorBitwiseAnd, REGEX("&")},
		{Token::Type::OperatorBitwiseOr, REGEX("\\|")},
		{Token::Type::OperatorBitwiseXor, REGEX("\\^")},
		{Token::Type::OperatorBitwiseNot, REGEX("~")},
		
		{Token::Type::OperatorShiftLeft, REGEX("<<")},
		{Token::Type::OperatorShiftRight, REGEX(">>")},
		
		{Token::Type::OperatorEqual, REGEX("==")},
		{Token::Type::OperatorNotEqual, REGEX("!=")},
		{Token::Type::OperatorLesser, REGEX("<")},
		{Token::Type::OperatorGreater, REGEX(">")},
		{Token::Type::OperatorLesserEqual, REGEX("<=")},
		{Token::Type::OperatorGreaterEqual, REGEX(">=")},
		
		{Token::Type::OperatorAnd, REGEX("&&")},
		{Token::Type::OperatorOr, REGEX("\\|\\|")},
		{Token::Type::OperatorXor, REGEX("\\^\\^")},
		{Token::Type::OperatorNot, REGEX("!")},
		
		{Token::Type::OperatorAssign, REGEX("=")},
		{Token::Type::OperatorAddAssign, REGEX("+=")},
		{Token::Type::OperatorSubAssign, REGEX("-=")},
		{Token::Type::OperatorMultAssign, REGEX("\\*=")},
		{Token::Type::OperatorDivAssign, REGEX("\\/=")},
		{Token::Type::OperatorModAssign, REGEX("%=")},
		{Token::Type::OperatorBitwiseAndAssign, REGEX("&=")},
		{Token::Type::OperatorBitwiseOrAssign, REGEX("\\|=")},
		{Token::Type::OperatorBitwiseXorAssign, REGEX("\\^=")},
		{Token::Type::OperatorBitwiseNotAssign, REGEX("~=")},
		{Token::Type::OperatorShiftLeftAssign, REGEX("<<=")},
		{Token::Type::OperatorShiftRightAssign, REGEX(">>=")},
		
		{Token::Type::Increment, REGEX("++")},
		{Token::Type::Decrement, REGEX("--")},
		
		{Token::Type::OperatorAddressOf, REGEX("\\?+")},
		{Token::Type::OperatorDereference, REGEX("@+")},
		
		{Token::Type::LineComment, REGEX("\\/\\/")}
	};*/
	
	#undef REGEX
}
