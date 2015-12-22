#include "Lexer.hpp"

#include <stdexcept>

namespace
{
	using TokenCheck = bool(const std::string& input);

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

namespace dbr
{
	namespace sl
	{
		Lexer::TokenCode Lexer::run(const std::string& input)
		{
			TokenCode tokens;

			// I probably need to go character by character in a string first. If I find a Token::Type::Symbol*,
			// I should split the string there. I could maybe even "pre-process" it. Before this for loop, iterate
			// over the input, and modify and insert strings into input as needed to do this.
			for(auto i = 0u; i < input.size(); ++i)
			{

			}

			return tokens;
		}
	}
}

namespace
{
	bool isIntegerValue(const std::string& input)
	{

	}

	bool isFloatValue(const std::string& input)
	{

	}

	bool isCharValue(const std::string& input)
	{

	}

	bool isStringValue(const std::string& input)
	{

	}

	bool isBoolValue(const std::string& input)
	{

	}

	bool isPointer(const std::string& input)
	{

	}

	bool isFunction(const std::string& input)
	{

	}

	bool isArray(const std::string& input)
	{

	}

	bool isMemoryAllocation(const std::string& input)
	{

	}

	bool isMemoryDeallocation(const std::string& input)
	{

	}

	bool isOperatorAdd(const std::string& input)
	{

	}

	bool isOperatorSub(const std::string& input)
	{

	}

	bool isOperatorMult(const std::string& input)
	{

	}

	bool isOperatorDiv(const std::string& input)
	{

	}

	bool isOperatorMod(const std::string& input)
	{

	}

	bool isOperatorBitwiseAnd(const std::string& input)
	{

	}

	bool isOperatorBitwiseOr(const std::string& input)
	{

	}

	bool isOperatorBitwiseXor(const std::string& input)
	{

	}

	bool isOperatorBitwiseNot(const std::string& input)
	{

	}

	bool isOperatorBitshiftLeft(const std::string& input)
	{

	}

	bool isOperatorBitshiftRight(const std::string& input)
	{

	}

	bool isOperatorEqual(const std::string& input)
	{

	}

	bool isOperatorNotEqual(const std::string& input)
	{

	}

	bool isOperatorLesser(const std::string& input)
	{

	}

	bool isOperatorGreater(const std::string& input)
	{

	}

	bool isOperatorLesserEqual(const std::string& input)
	{

	}

	bool isOperatorGreaterEqual(const std::string& input)
	{

	}

	bool isOperatorAnd(const std::string& input)
	{

	}

	bool isOperatorOr(const std::string& input)
	{

	}

	bool isOperatorXor(const std::string& input)
	{

	}

	bool isOperatorNot(const std::string& input)
	{

	}

	bool isOperatorAssign(const std::string& input)
	{

	}

	bool isOperatorAddAssign(const std::string& input)
	{

	}

	bool isOperatorSubAssign(const std::string& input)
	{

	}

	bool isOperatorMultAssign(const std::string& input)
	{

	}

	bool isOperatorDivAssign(const std::string& input)
	{

	}

	bool isOperatorModAssign(const std::string& input)
	{

	}

	bool isOperatorBitwiseAndAssign(const std::string& input)
	{

	}

	bool isOperatorBitwiseOrAssign(const std::string& input)
	{

	}

	bool isOperatorBitXorAssign(const std::string& input)
	{

	}

	bool isOperatorBitwiseNotAssign(const std::string& input)
	{

	}

	bool isOperatorBitshiftLeftAssign(const std::string& input)
	{

	}

	bool isOperatorBitshiftRightAssign(const std::string& input)
	{

	}

	bool isOperatorIncrement(const std::string& input)
	{

	}

	bool isOperatorDecrement(const std::string& input)
	{

	}

	bool isOperatorAddressOf(const std::string& input)
	{

	}

	bool isOperatorDereference(const std::string& input)
	{

	}

	bool isLineComment(const std::string& input)
	{

	}

	bool isBlockComment(const std::string& input)
	{

	}

	bool isIdentifier(const std::string& input)
	{

	}
}
