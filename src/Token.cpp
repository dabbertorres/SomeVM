#include "Token.hpp"

namespace lng
{
	Token::Token(Type t, const std::string& v)
	:	type(t),
		value(v)
	{}
	
	const std::map<Token::Type, std::string> Token::typeMap = 
	{
		{Type::ValueInteger, "ValueInteger"},
		{Type::ValueFloat, "ValueFloat"},
		{Type::ValueChar, "ValueChar"},
		{Type::ValueString, "ValueString"},
		{Type::ValueBool, "ValueBool"},
		
		{Type::TypeInteger, "TypeInteger"},
		{Type::TypeFloat, "TypeFloat"},
		{Type::TypeChar, "TypeChar"},
		{Type::TypeString, "TypeString"},
		{Type::TypeBool, "TypeBool"},
		{Type::TypeObject, "TypeObject"},
		{Type::TypeFunction, "TypeFunction"},
		{Type::TypeArray, "TypeArray"},
		
		{Type::Identifier, "Identifier"},
		
		{Type::MemoryAllocate, "MemoryAllocate"},
		{Type::MemoryDeallocate, "MemoryDeallocate"},
		
		{Type::Pointer, "Pointer"},
		
		{Type::OperatorAdd, "OperatorAdd"},
		{Type::OperatorSub, "OperatorSub"},
		{Type::OperatorMult, "OperatorMult"},
		{Type::OperatorDiv, "OperatorDiv"},
		{Type::OperatorMod, "OperatorMod"},
		
		{Type::OperatorBitwiseAnd, "OperatorBitwiseAnd"},
		{Type::OperatorBitwiseOr, "OperatorBitwiseOr"},
		{Type::OperatorBitwiseXor, "OperatorBitwiseXor"},
		{Type::OperatorBitwiseNot, "OperatorBitwiseNot"},
		
		{Type::OperatorShiftLeft, "OperatorShiftLeft"},
		{Type::OperatorShiftRight, "OperatorShiftRight"},
		
		{Type::OperatorEqual, "OperatorEqual"},
		{Type::OperatorNotEqual, "OperatorNotEqual"},
		{Type::OperatorLesser, "OperatorLesser"},
		{Type::OperatorGreater, "OperatorGreater"},
		{Type::OperatorLesserEqual, "OperatorLesserEqual"},
		{Type::OperatorGreaterEqual, "OperatorGreaterEqual"},
		
		{Type::OperatorAnd, "OperatorAnd"},
		{Type::OperatorOr, "OperatorOr"},
		{Type::OperatorXor, "OperatorXor"},
		{Type::OperatorNot, "OperatorNot"},
		
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
		{Type::OperatorShiftLeftAssign, "OperatorShiftLeftAssign"},
		{Type::OperatorShiftRightAssign, "OperatorShiftRightAssign"},
		
		{Type::Increment, "Increment"},
		{Type::Decrement, "Decrement"},
		
		{Type::OperatorAddressOf, "OperatorAddressOf"},
		{Type::OperatorDereference, "OperatorDereference"},
		
		{Type::LineComment, "LineComment"},
		{Type::BlockComment, "BlockComment"},
		
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
	};
}
