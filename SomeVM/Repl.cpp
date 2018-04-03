#include "Repl.hpp"

#include <istream>
#include <ostream>

Repl::Repl(std::istream& input, std::ostream& output)
    : input{ input },
    output{ output }
{}

Repl::~Repl()
{}

void Repl::run()
{

}
