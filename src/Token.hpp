#ifndef LNG_TOKEN_HPP
#define LNG_TOKEN_HPP

#include <string>
#include <utility>

namespace dbr
{
	namespace svm
	{
		namespace lex
		{
			enum class TokenType
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

			struct Pair
			{
				Pair() = delete;
				constexpr Pair(TokenType t, const char* n)
				:	type(t),
					name(n)
				{}

				TokenType type;
				const char* name;
			};
			
			class Token
			{
				public:
					Token(TokenType t);

					const TokenType type;

					static constexpr const char* typeToString(TokenType t)
					{
						return typeToStringHelper(t, 0);
					}

				private:
					static constexpr const char* typeToStringHelper(TokenType t, std::size_t idx)
					{
						return typeMap[idx].type == t ? typeMap[idx].name : typeToStringHelper(t, ++idx);
					}

					static constexpr Pair typeMap[] = 
					{
						// Primitive Values
						{TokenType::ValueInteger, "ValueInteger"},
						{TokenType::ValueFloat, "ValueFloat"},
						{TokenType::ValueChar, "ValueChar"},
						{TokenType::ValueString, "ValueString"},
						{TokenType::ValueBool, "ValueBool"},

						// TokenType modifiers
						{TokenType::TypePointer, "TypePointer"},
						{TokenType::TypeFunction, "TypeFunction"},
						{TokenType::TypeArray, "TypeArray"},

						// Memory Operations
						{TokenType::MemoryAllocate, "MemoryAllocate"},
						{TokenType::MemoryDeallocate, "MemoryDeallocate"},

						/* Operators */

						// arithmetic
						{TokenType::OperatorAdd, "OperatorAdd"},
						{TokenType::OperatorSub, "OperatorSub"},
						{TokenType::OperatorMult, "OperatorMult"},
						{TokenType::OperatorDiv, "OperatorDiv"},
						{TokenType::OperatorMod, "OperatorMod"},

						// Bitwise
						{TokenType::OperatorBitwiseAnd, "OperatorBitwiseAnd"},
						{TokenType::OperatorBitwiseOr, "OperatorBitwiseOr"},
						{TokenType::OperatorBitwiseXor, "OperatorBitwiseXor"},
						{TokenType::OperatorBitwiseNot, "OperatorBitwiseNot"},

						// Bitshift
						{TokenType::OperatorBitshiftLeft, "OperatorBitshiftLeft"},
						{TokenType::OperatorBitshiftRight, "OperatorBitshiftRight"},

						// Relational
						{TokenType::OperatorEqual, "OperatorEqual"},
						{TokenType::OperatorNotEqual, "OperatorNotEqual"},
						{TokenType::OperatorLesser, "OperatorLesser"},
						{TokenType::OperatorGreater, "OperatorGreater"},
						{TokenType::OperatorLesserEqual, "OperatorLesserEqual"},
						{TokenType::OperatorGreaterEqual, "OperatorGreaterEqual"},

						// Logical
						{TokenType::OperatorAnd, "OperatorAnd"},
						{TokenType::OperatorOr, "OperatorOr"},
						{TokenType::OperatorXor, "OperatorXor"},
						{TokenType::OperatorNot, "OperatorNot"},

						// Assignment
						{TokenType::OperatorAssign, "OperatorAssign"},
						{TokenType::OperatorAddAssign, "OperatorAddAssign"},
						{TokenType::OperatorSubAssign, "OperatorSubAssign"},
						{TokenType::OperatorMultAssign, "OperatorMultAssign"},
						{TokenType::OperatorDivAssign, "OperatorDivAssign"},
						{TokenType::OperatorModAssign, "OperatorModAssign"},
						{TokenType::OperatorBitwiseAndAssign, "OperatorBitwiseAndAssign"},
						{TokenType::OperatorBitwiseOrAssign, "OperatorBitwiseOrAssign"},
						{TokenType::OperatorBitwiseXorAssign, "OperatorBitwiseXorAssign"},
						{TokenType::OperatorBitwiseNotAssign, "OperatorBitwiseNotAssign"},
						{TokenType::OperatorBitshiftLeftAssign, "OperatorBitshiftLeftAssign"},
						{TokenType::OperatorBitshiftRightAssign, "OperatorBitshiftRightAssign"},

						// increment/decrement
						{TokenType::Increment, "Increment"},
						{TokenType::Decrement, "Decrement"},

						// Memory
						{TokenType::OperatorAddressOf, "OperatorAddressOf"},
						{TokenType::OperatorDereference, "OperatorDereference"},

						// Comments
						{TokenType::LineComment, "LineComment"},
						{TokenType::BlockComment, "BlockComment"},

						// Symbols
						{TokenType::SymbolOpenParen, "SymbolOpenParen"},
						{TokenType::SymbolEndParen, "SymbolEndParen"},
						{TokenType::SymbolOpenSquareBracket, "SymbolOpenSquareBracket"},
						{TokenType::SymbolEndSquareBracket, "SymbolEndSquareBracket"},
						{TokenType::SymbolOpenCurlyBrace, "SymbolOpenCurlyBrace"},
						{TokenType::SymbolEndCurlyBrace, "SymbolEndCurlyBrace"},
						{TokenType::SymbolOpenAngleBracket, "SymbolOpenAngleBracket"},
						{TokenType::SymbolEndAngleBracket, "SymbolEndAngleBracket"},
						{TokenType::SymbolSingleQuote, "SymbolSingleQuote"},
						{TokenType::SymbolDoubleQuote, "SymbolDoubleQuote"},

						{TokenType::Identifier, "Identifier"},
					};
			};
		}
	}
}

#endif
