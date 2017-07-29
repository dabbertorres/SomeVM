#pragma once

#include <string>
#include <iosfwd>

namespace sl
{
    struct Token
    {
        enum class Type
        {
            // literals
            Number,
            String,
            Bool,

            // symbols
            Semicolon,
            Colon,
            Comma,
            ParenLeft,
            ParenRight,
            Newline,
            Dollar,

            // keywords
            Func,
            If,
            Switch,
            Case,
            Else,
            While,

            Identifier,

            End,

            Unknown,
        };

        std::string value;
        Type type;
        size_t line;
        size_t position;

        Token(size_t line, size_t position);
        Token(std::string value, Type type, size_t line, size_t position);

        bool valid() const;
    };

    std::ostream& operator<<(std::ostream& os, const Token& t);
    std::ostream& operator<<(std::ostream& os, Token::Type t);
}
