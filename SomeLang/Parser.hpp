#pragma once

#include <stdexcept>
#include <sstream>
#include <vector>

#include "Token.hpp"
#include "Statement.hpp"

namespace sl
{
    std::vector<Statement> parse(const std::string& stream, const std::vector<Token>& tokens);

    class ParseError : public std::exception
    {
    public:
        ParseError(const std::string& stream, const Token& tok, const std::string& context, const std::string& expected);
        ~ParseError() = default;

        const char* what() const override;

    private:
        std::string error;
    };
}
