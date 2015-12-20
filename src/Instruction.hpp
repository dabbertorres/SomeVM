#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

using byte = unsigned char;
	
namespace lng
{
	class Instruction
	{
		public:
			enum class Type
			{
				// memory ops
				PushLiteral,
				PushVariable,
				Pop,
				Store,
				Erase,

				// math ops
				Add,
				Sub,
				Mult,
				Div,
				Mod,

				// comparison
				Not,
				Lesser,
				LesserOrEqual,
				Equal,
				// Greater = Not LesserOrEqual
				// GreaterOrEqual = Not Lesser
				// NotEqual = Not Equal

				// conditions
				If,

				// branching
				Goto,
				Return,

				End,
			};

			Instruction();
			
		private:
	};
}

#endif // INSTRUCTIONS_HPP
