#pragma once

#include <iosfwd>
#include <unordered_map>

#include "libSomeVM/Program.hpp"
#include "libSomeVM/Instruction.hpp"

#include "Statement.hpp"

namespace sl
{
    svm::Program assemble(const std::vector<Statement>& stmts);

	class Assembler
	{
	public:
		Assembler() = delete;
		~Assembler() = delete;

		static svm::Program run(std::istream& in, std::ostream& out);

		// adds to program, rather than creating a whole new Program
		static void run(std::istream& in, std::ostream& out, svm::Program& program);

		static bool isRegister(const std::string& str);
		static std::uint16_t toRegister(const std::string& regStr);

	private:
		static const std::unordered_map<std::string, svm::Instruction(*)(std::istream&, svm::Program&)> commands;
	};
}
