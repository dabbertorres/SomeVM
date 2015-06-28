#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace lng
{
	class Token
	{
		public:
			enum class Type : int
			{
				// Primitive Values
				ValueInteger,
				ValueFloat,
				ValueChar,
				ValueString,
				ValueBool,
				
				// Types
				TypeInteger,
				TypeFloat,
				TypeChar,
				TypeString,
				TypeBool,
				TypeObject,
				TypeFunction,
				TypeArray,
				
				Identifier,
				
				// Memory Operations
				MemoryAllocate,
				MemoryDeallocate,
				
				// Type Modifiers
				Pointer,
				
				/* Operators */
				// arithmetic
				OperatorAdd,
				OperatorSub,
				OperatorMult,
				OperatorDiv,
				OperatorMod,
				
				// Bitwise
				OperatorBitwiseAnd,
				OperatorBitwiseOr,
				OperatorBitwiseXor,
				OperatorBitwiseNot,
				
				// Bitshift
				OperatorShiftLeft,
				OperatorShiftRight,
				
				// Relational
				OperatorEqual,
				OperatorNotEqual,
				OperatorLesser,
				OperatorGreater,
				OperatorLesserEqual,
				OperatorGreaterEqual,
				
				// Logical
				OperatorAnd,
				OperatorOr,
				OperatorXor,
				OperatorNot,
				
				// Assignment
				OperatorAssign,
				OperatorAddAssign,
				OperatorSubAssign,
				OperatorMultAssign,
				OperatorDivAssign,
				OperatorModAssign,
				OperatorBitwiseAndAssign,
				OperatorBitwiseOrAssign,
				OperatorBitwiseXorAssign,
				OperatorBitwiseNotAssign,
				OperatorShiftLeftAssign,
				OperatorShiftRightAssign,
				
				// increment/decrement
				Increment,
				Decrement,
				
				// Memory Operations
				OperatorAddressOf,
				OperatorDereference,
				
				// Commentss
				LineComment,
				
				// Symbols
				SymbolOpenParen,
				SymbolEndParen,
				SymbolOpenSquareBracket,
				SymbolEndSquareBracket,
				SymbolOpenCurlyBrace,
				SymbolEndCurlyBrace,
				SymbolOpenAngleBracket,
				SymbolEndAngleBracket,
			};

			Token(Type t, const std::string& v = "");
			
			const Type type;
			const std::string value;
	};
}

#endif // TOKEN_HPP
