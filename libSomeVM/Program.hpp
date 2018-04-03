#pragma once

#include <cstdint>
#include <vector>
#include <iosfwd>

#include "Function.hpp"
#include "Value.hpp"

namespace svm
{
	struct Program
	{
        Program();
        Program(Program&&) = default;
        ~Program() = default;

		std::vector<Value> constants;
		std::vector<Function> functions;

        // TODO (optionally) store program state (ie: registry, call stack, etc)

		// return the amount of bytes read/written
		uint64_t load(std::istream& input);
		uint64_t write(std::ostream& output) const;
	};
}

