#ifndef LNG_TOKEN_HPP
#define LNG_TOKEN_HPP

#include <string>
#include <map>

namespace lng
{
	class Token
	{
		public:
			enum class Type
			{
				// Primitive Values
				ValueInteger,
				ValueFloat,
				ValueChar,
				ValueString,
				ValueBool,
				
				// Type modifiers
				TypePointer,
				TypeFunction,
				TypeArray,
				
				// Memory Operations
				MemoryAllocate,
				MemoryDeallocate,
				
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
				OperatorBitshiftLeft,
				OperatorBitshiftRight,
				
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
				OperatorBitshiftLeftAssign,
				OperatorBitshiftRightAssign,
				
				// increment/decrement
				Increment,
				Decrement,
				
				// Memory
				OperatorAddressOf,
				OperatorDereference,
				
				// Comments
				LineComment,
				BlockComment,
				
				// Symbols
				SymbolOpenParen,
				SymbolEndParen,
				SymbolOpenSquareBracket,
				SymbolEndSquareBracket,
				SymbolOpenCurlyBrace,
				SymbolEndCurlyBrace,
				SymbolOpenAngleBracket,
				SymbolEndAngleBracket,
				SymbolSingleQuote,
				SymbolDoubleQuote,
				
				Identifier,
			};

			Token(Type t, const std::string& v = "");
			
			const Type type;
			const std::string value;
			
			const static std::map<Type, std::string> typeMap;
	};
}

#endif
