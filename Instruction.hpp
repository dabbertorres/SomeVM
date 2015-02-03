#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

using byte = unsigned char;
	
namespace lng
{
	enum class Instruction : byte
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
		Greater,
		Lesser,
		GreaterOrEqual,
		LesserOrEqual,
		Equal,
		NotEqual,
		// branching
		If,
	    Goto,
		Return,
		End
	};
}

#endif // INSTRUCTIONS_HPP
