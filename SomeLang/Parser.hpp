#pragma once

#include <stdexcept>
#include <string>
#include <sstream>
#include <iosfwd>
#include <vector>

#include "Token.hpp"
#include "Statement.hpp"

namespace sl
{
    std::vector<Statement> parse(const std::string& streamName, std::istream& stream, const std::vector<Token>& tokens);

    class ParseError : public std::exception
    {
    public:
        ParseError(const std::string& error);
        ParseError(const std::string& streamName, const Token& tok, std::istream& stream, const std::string& expected);
        ~ParseError() = default;

        const char* what() const noexcept override;

    private:
        std::string error;
        
        static std::string getLine(const Token& tok, std::istream& stream);
    };
}
