#pragma once

#include <iosfwd>
#include <stdexcept>
#include <string>
#include <vector>

#include "Token.hpp"
#include "Statement.hpp"

namespace sl
{
    std::vector<Statement> parse(const std::string& streamName, std::istream& inputStream, std::ostream& logStream, const std::vector<Token>& tokens);
}
