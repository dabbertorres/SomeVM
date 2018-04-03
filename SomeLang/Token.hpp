#pragma once

#include <cstddef>
#include <string>
#include <iosfwd>

namespace sl
{
    struct Token
    {
        enum class Type
        {
            // literals
            Float,
            Int,
            String,
            Bool,

            // symbols
            Semicolon,
            Colon,
            Comma,
            ParenOpen,
            ParenClose,
            Newline,
            Dollar,
            Arrow,

            // keywords
            Func,
            If,
            Switch,
            Case,
            Else,
            While,

            Identifier,

            Unknown,
        };

        std::string lexeme;
        Type type;
        size_t line;
        size_t position;

        Token();
        Token(size_t line, size_t position);
        Token(std::string lexeme, Type type, size_t line, size_t position);

        bool valid() const;
    };
}

std::ostream& operator<<(std::ostream& os, const sl::Token& t);
std::ostream& operator<<(std::ostream& os, sl::Token::Type t);
