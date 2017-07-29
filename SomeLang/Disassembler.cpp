#include "Disassembler.hpp"

#include <ostream>

#include "libSomeVM/Program.hpp"
#include "libSomeVM/Instruction.hpp"

namespace sl
{
	void disassemble(const svm::Program& prog, std::ostream& output)
	{
		for (auto& c : prog.constants)
		{

		}
	}
}
