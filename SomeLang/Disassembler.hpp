#ifndef DBR_SVM_DISASSEMBLER_HPP
#define DBR_SVM_DISASSEMBLER_HPP

#include <iosfwd>

namespace svm
{
	struct Program;
}

namespace sl
{
    void disassemble(const svm::Program& prog, std::ostream& os);
}

#endif
