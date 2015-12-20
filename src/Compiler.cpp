#include "Compiler.hpp"

#include <iostream>

namespace lng
{
	Compiler::Bytecode Compiler::run(const NewParser::Parsed& parsed)
	{
		Bytecode code;

		std::cout << "\tCode:\n";
		for(auto& p : parsed)
		{

		}
		
		code.push_back(static_cast<byte>(Instruction::End));

		return code;
	}
}
