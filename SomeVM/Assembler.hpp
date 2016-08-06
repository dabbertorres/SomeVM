#ifndef DBR_SVM_ASSEMBLER_HPP
#define DBR_SVM_ASSEMBLER_HPP

#include <istream>
#include <tuple>
#include <unordered_map>

#include "Instruction.hpp"
#include "Value.hpp"

namespace dbr
{
	namespace svm
	{
		struct Program;

		class Assembler
		{
		public:
			Assembler() = delete;
			~Assembler() = delete;

			static Program run(std::istream& in, std::ostream& out);

			// adds to program, rather than creating a whole new Program
			static void run(std::istream& in, std::ostream& out, Program& program);

			static bool isRegister(const std::string& str);
			static std::uint16_t toRegister(const std::string& regStr);

		private:
			static const std::unordered_map<std::string, dbr::svm::Instruction(*)(std::istream&, Program&)> commands;
		};
	}
}

#endif
