#ifndef DBR_SVM_IL_PARSER_HPP
#define DBR_SVM_IL_PARSER_HPP

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

		namespace il
		{
			class Parser
			{
				public:
					Parser() = delete;
					~Parser() = delete;

					static Program run(std::istream& in, std::ostream& out);
					static void run(std::istream& in, std::ostream& out, Program& program);

					static bool isRegister(const std::string& str);
					static bool isConst(const std::string& str);
					static std::size_t toRegister(const std::string& regStr);
					static std::size_t toConst(const std::string& regStr);

				private:
					// memory ops
					static Instruction load(std::istream& in);
					static void constant(std::istream& in, Constants& constants);

					// math ops
					static Instruction add(std::istream& in);
					static Instruction sub(std::istream& in);
					static Instruction mult(std::istream& in);
					static Instruction div(std::istream& in);
					static Instruction mod(std::istream& in);
					static Instruction neg(std::istream& in);

					// comparison
					static Instruction not(std::istream& in);
					static Instruction less(std::istream& in);
					static Instruction lesseq(std::istream& in);
					static Instruction gret(std::istream& in);
					static Instruction greteq(std::istream& in);
					static Instruction eq(std::istream& in);
					static Instruction neq(std::istream& in);

					// conditions
					static Instruction ifTest(std::istream& in);

					// branching
					static Instruction call(std::istream& in);
					static Instruction ret(std::istream& in);
					static Instruction jump(std::istream& in);

					// misc
					static Instruction noop(std::istream& in);
					static Instruction print(std::istream& in);

					static const std::unordered_map<std::string, dbr::svm::Instruction(*)(std::istream&)> commands;
			};
		}
	}
}

#endif
