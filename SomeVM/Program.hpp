#ifndef DBR_SVM_PROGRAM_HPP
#define DBR_SVM_PROGRAM_HPP

#include <vector>

#include "Function.hpp"
#include "Value.hpp"

namespace dbr
{
	namespace svm
	{
		using Registry = std::vector<Value>;

		struct Program
		{
			Registry constants;
			std::vector<Function> functions;

			void load(const std::string& file);
			void write(const std::string& file);

			static constexpr const char* BINARY_ID = ".svm";
			static constexpr std::uint32_t VERSION = 0;
		};
	}
}

#endif
