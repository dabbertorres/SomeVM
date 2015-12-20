#ifndef LNG_COMPILER_HPP
#define LNG_COMPILER_HPP

#include <vector>

#include "Instruction.hpp"
#include "NewParser.hpp"

namespace lng
{
	class Compiler
	{
		public:
			using Bytecode = std::vector<byte>;

			Compiler() = default;

			Bytecode run(const NewParser::Parsed&);
	};
}

#endif	// LNG_COMPILER_HPP