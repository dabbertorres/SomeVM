#include "Lexer.hpp"

#include <stdexcept>

namespace
{
	using TokenCheck = bool(const std::string&);

	static TokenCheck isIntegerValue;
	static TokenCheck isFloatValue;
	static TokenCheck isCharValue;
	static TokenCheck isStringValue;
	static TokenCheck isBoolValue;

	static TokenCheck isPointer;
	static TokenCheck isFunction;
	static TokenCheck isArray;

	static TokenCheck isMemoryAllocation;
	static TokenCheck isMemoryDeallocation;
	
	static TokenCheck isOperatorAdd;
	static TokenCheck isOperatorSub;
	static TokenCheck isOperatorMult;
	static TokenCheck isOperatorDiv;
	static TokenCheck isOperatorMod;

	static TokenCheck isOperatorBitwiseAnd;
	static TokenCheck isOperatorBitwiseOr;
	static TokenCheck isOperatorBitwiseXor;
	static TokenCheck isOperatorBitwiseNot;

	static TokenCheck isOperatorBitshiftLeft;
	static TokenCheck isOperatorBitshiftRight;

	static TokenCheck isOperatorEqual;
	static TokenCheck isOperatorNotEqual;
	static TokenCheck isOperatorLesser;
	static TokenCheck isOperatorGreater;
	static TokenCheck isOperatorLesserEqual;
	static TokenCheck isOperatorGreaterEqual;

	static TokenCheck isOperatorAnd;
	static TokenCheck isOperatorOr;
	static TokenCheck isOperatorXor;
	static TokenCheck isOperatorNot;

	static TokenCheck isOperatorAssign;
	static TokenCheck isOperatorAddAssign;
	static TokenCheck isOperatorSubAssign;
	static TokenCheck isOperatorMultAssign;
	static TokenCheck isOperatorDivAssign;
	static TokenCheck isOperatorModAssign;
	static TokenCheck isOperatorBitwiseAndAssign;
	static TokenCheck isOperatorBitwiseOrAssign;
	static TokenCheck isOperatorBitXorAssign;
	static TokenCheck isOperatorBitwiseNotAssign;
	static TokenCheck isOperatorBitshiftLeftAssign;
	static TokenCheck isOperatorBitshiftRightAssign;

	static TokenCheck isOperatorIncrement;
	static TokenCheck isOperatorDecrement;

	static TokenCheck isOperatorAddressOf;
	static TokenCheck isOperatorDereference;

	static TokenCheck isLineComment;
	static TokenCheck isBlockComment;

	static TokenCheck isIdentifier;
}

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

static bool isIntegerValue(const std::string& input)
{

}

static TokenCheck isFloatValue;
static TokenCheck isCharValue;
static TokenCheck isStringValue;
static TokenCheck isBoolValue;

static TokenCheck isPointer;
static TokenCheck isFunction;
static TokenCheck isArray;

static TokenCheck isMemoryAllocation;
static TokenCheck isMemoryDeallocation;

static TokenCheck isOperatorAdd;
static TokenCheck isOperatorSub;
static TokenCheck isOperatorMult;
static TokenCheck isOperatorDiv;
static TokenCheck isOperatorMod;

static TokenCheck isOperatorBitwiseAnd;
static TokenCheck isOperatorBitwiseOr;
static TokenCheck isOperatorBitwiseXor;
static TokenCheck isOperatorBitwiseNot;

static TokenCheck isOperatorBitshiftLeft;
static TokenCheck isOperatorBitshiftRight;

static TokenCheck isOperatorEqual;
static TokenCheck isOperatorNotEqual;
static TokenCheck isOperatorLesser;
static TokenCheck isOperatorGreater;
static TokenCheck isOperatorLesserEqual;
static TokenCheck isOperatorGreaterEqual;

static TokenCheck isOperatorAnd;
static TokenCheck isOperatorOr;
static TokenCheck isOperatorXor;
static TokenCheck isOperatorNot;

static TokenCheck isOperatorAssign;
static TokenCheck isOperatorAddAssign;
static TokenCheck isOperatorSubAssign;
static TokenCheck isOperatorMultAssign;
static TokenCheck isOperatorDivAssign;
static TokenCheck isOperatorModAssign;
static TokenCheck isOperatorBitwiseAndAssign;
static TokenCheck isOperatorBitwiseOrAssign;
static TokenCheck isOperatorBitXorAssign;
static TokenCheck isOperatorBitwiseNotAssign;
static TokenCheck isOperatorBitshiftLeftAssign;
static TokenCheck isOperatorBitshiftRightAssign;

static TokenCheck isOperatorIncrement;
static TokenCheck isOperatorDecrement;

static TokenCheck isOperatorAddressOf;
static TokenCheck isOperatorDereference;

static TokenCheck isLineComment;
static TokenCheck isBlockComment;

static TokenCheck isIdentifier;
