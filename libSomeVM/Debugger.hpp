#pragma once

#include "libSomeVM.hpp"

#include <cstdint>
#include <vector>

namespace svm
{
	struct SVM_API Breakpoint
	{
		std::uint64_t functionIndex;
		std::uint64_t instructionIndex;
	};

	class SVM_API Debugger
	{
	public:
		Debugger();
		~Debugger();

		void run(const std::vector<Breakpoint>& breakpoints);
	};
}
