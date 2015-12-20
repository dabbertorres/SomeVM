#include "Token.hpp"

namespace lng
{
	Token::Token(Type t, const std::string& v)
	:	type(t),
		value(v)
	{}
	
	const std::map<Token::Type, std::string> Token::typeMap = 
	{
		// Primitive Values
		{Type::ValueInteger, "ValueInteger"},
		{Type::ValueFloat, "ValueFloat"},
		{Type::ValueChar, "ValueChar"},
		{Type::ValueString, "ValueString"},
		{Type::ValueBool, "ValueBool"},

		// Type modifiers
		{Type::TypePointer, "TypePointer"},
		{Type::TypeFunction, "TypeFunction"},
		{Type::TypeArray, "TypeArray"},

		// Memory Operations
		{Type::MemoryAllocate, "MemoryAllocate"},
		{Type::MemoryDeallocate, "MemoryDeallocate"},

		/* Operators */

		// arithmetic
		{Type::OperatorAdd, "OperatorAdd"},
		{Type::OperatorSub, "OperatorSub"},
		{Type::OperatorMult, "OperatorMult"},
		{Type::OperatorDiv, "OperatorDiv"},
		{Type::OperatorMod, "OperatorMod"},

		// Bitwise
		{Type::OperatorBitwiseAnd, "OperatorBitwiseAnd"},
		{Type::OperatorBitwiseOr, "OperatorBitwiseOr"},
		{Type::OperatorBitwiseXor, "OperatorBitwiseXor"},
		{Type::OperatorBitwiseNot, "OperatorBitwiseNot"},

		// Bitshift
		{Type::OperatorBitshiftLeft, "OperatorBitshiftLeft"},
		{Type::OperatorBitshiftRight, "OperatorBitshiftRight"},

		// Relational
		{Type::OperatorEqual, "OperatorEqual"},
		{Type::OperatorNotEqual, "OperatorNotEqual"},
		{Type::OperatorLesser, "OperatorLesser"},
		{Type::OperatorGreater, "OperatorGreater"},
		{Type::OperatorLesserEqual, "OperatorLesserEqual"},
		{Type::OperatorGreaterEqual, "OperatorGreaterEqual"},

		// Logical
		{Type::OperatorAnd, "OperatorAnd"},
		{Type::OperatorOr, "OperatorOr"},
		{Type::OperatorXor, "OperatorXor"},
		{Type::OperatorNot, "OperatorNot"},

		// Assignment
		{Type::OperatorAssign, "OperatorAssign"},
		{Type::OperatorAddAssign, "OperatorAddAssign"},
		{Type::OperatorSubAssign, "OperatorSubAssign"},
		{Type::OperatorMultAssign, "OperatorMultAssign"},
		{Type::OperatorDivAssign, "OperatorDivAssign"},
		{Type::OperatorModAssign, "OperatorModAssign"},
		{Type::OperatorBitwiseAndAssign, "OperatorBitwiseAndAssign"},
		{Type::OperatorBitwiseOrAssign, "OperatorBitwiseOrAssign"},
		{Type::OperatorBitwiseXorAssign, "OperatorBitwiseXorAssign"},
		{Type::OperatorBitwiseNotAssign, "OperatorBitwiseNotAssign"},
		{Type::OperatorBitshiftLeftAssign, "OperatorBitshiftLeftAssign"},
		{Type::OperatorBitshiftRightAssign, "OperatorBitshiftRightAssign"},

		// increment/decrement
		{Type::Increment, "Increment"},
		{Type::Decrement, "Decrement"},

		// Memory
		{Type::OperatorAddressOf, "OperatorAddressOf"},
		{Type::OperatorDereference, "OperatorDereference"},

		// Comments
		{Type::LineComment, "LineComment"},
		{Type::BlockComment, "BlockComment"},

		// Symbols
		{Type::SymbolOpenParen, "SymbolOpenParen"},
		{Type::SymbolEndParen, "SymbolEndParen"},
		{Type::SymbolOpenSquareBracket, "SymbolOpenSquareBracket"},
		{Type::SymbolEndSquareBracket, "SymbolEndSquareBracket"},
		{Type::SymbolOpenCurlyBrace, "SymbolOpenCurlyBrace"},
		{Type::SymbolEndCurlyBrace, "SymbolEndCurlyBrace"},
		{Type::SymbolOpenAngleBracket, "SymbolOpenAngleBracket"},
		{Type::SymbolEndAngleBracket, "SymbolEndAngleBracket"},
		{Type::SymbolSingleQuote, "SymbolSingleQuote"},
		{Type::SymbolDoubleQuote, "SymbolDoubleQuote"},

		{Type::Identifier, "Identifier"},
	};
}
