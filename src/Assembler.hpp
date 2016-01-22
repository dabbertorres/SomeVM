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
			class Assembler
			{
				public:
					Assembler() = delete;
					~Assembler() = delete;

					static Program run(std::istream& in, std::ostream& out);
					static void run(std::istream& in, std::ostream& out, Program& program);

					static bool isRegister(const std::string& str);

					static bool isInt(const std::string& str);
					static bool isFloat(const std::string& str);
					static bool isBool(const std::string& str);

					static std::size_t toRegister(const std::string& regStr);

					static bool strToInt(const std::string& str, Int& i);
					static bool strToFloat(const std::string& str, Float& i);
					static bool strToBool(const std::string& str, Bool& i);

				private:
					static const std::unordered_map<std::string, dbr::svm::Instruction(*)(std::istream&)> commands;
			};
		}
	}
}

#endif
