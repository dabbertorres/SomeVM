#pragma once

#include <vector>
#include <string>
#include <iosfwd>

#include "Token.hpp"

namespace svm
{
	std::vector<Token> lex(std::istream& in);
}
