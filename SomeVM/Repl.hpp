#pragma once

#include <iosfwd>

#include "libSomeVM/Value.hpp"
#include "libSomeVM/VM.hpp"

namespace svm
{
	class Repl
	{
	public:
		Repl(std::istream& input, std::ostream& output);
		~Repl();

		//void print(const Value& val) const;

	private:
		std::istream& input;
		std::ostream& output;
	};
}
