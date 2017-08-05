#pragma once

#include <cstdint>
#include <vector>

namespace svm
{
	struct Breakpoint
	{
		std::uint64_t functionIndex;
		std::uint64_t instructionIndex;
	};

	class Debugger
	{
	public:
		Debugger();
		~Debugger();

		void run(const std::vector<Breakpoint>& breakpoints);
	};
}
