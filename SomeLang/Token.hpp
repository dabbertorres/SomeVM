#ifndef DBR_SVM_IL_TOKEN_HPP
#define DBR_SVM_IL_TOKEN_HPP

#include <cstdint>
#include <cstddef>

namespace dbr
{
	namespace sl
	{
		using byte = std::uint8_t;

		class Token
		{
			public:
				enum class Type : std::size_t
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

				Token(Type t);

				const Type type;

				const char* asString();
		};
	}
}

#endif
