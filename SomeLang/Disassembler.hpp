#pragma once

#include <iosfwd>

namespace svm
{
	struct Program;
}

namespace sl
{
    void disassemble(const svm::Program& prog, std::ostream& os);
}
