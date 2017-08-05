#include "Token.hpp"

#include <ostream>

namespace sl
{
    Token::Token(size_t line, size_t position)
        : value{},
        type{ Type::Unknown },
        line{ line },
        position{ position }
    {}

    Token::Token(std::string value, Type type, size_t line, size_t position)
        : value{ value },
        type{ type },
        line{ line },
        position{ position }
    {}

    bool Token::valid() const
    {
        return type != Type::Unknown;
    }

    std::ostream& operator<<(std::ostream& os, const Token& t)
    {
        os << "Token:\n"
            << "{\n"
            << "\tvalue:     " << t.value << '\n'
            << "\ttype:      " << t.type << '\n'
            << "\tline:      " << t.line << '\n'
            << "\tcharacter: " << t.position << '\n'
            << "}\n";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, Token::Type t)
    {
        switch (t)
        {
            case Token::Type::Number:
                return os << "Number";

            case Token::Type::String:
                return os << "String";

            case Token::Type::Bool:
                return os << "Bool";

            case Token::Type::Semicolon:
                return os << "Semicolon";

            case Token::Type::Colon:
                return os << "Colon";

            case Token::Type::Comma:
                return os << "Comma";

            case Token::Type::ParenLeft:
                return os << "ParenLeft";

            case Token::Type::ParenRight:
                return os << "ParenRight";

            case Token::Type::Newline:
                return os << "Newline";

            case Token::Type::Dollar:
                return os << "Dollar";

            case Token::Type::Func:
                return os << "Func";

            case Token::Type::If:
                return os << "If";

            case Token::Type::Switch:
                return os << "Switch";

            case Token::Type::Case:
                return os << "Class";

            case Token::Type::Else:
                return os << "Else";

            case Token::Type::While:
                return os << "While";

            case Token::Type::Identifier:
                return os << "Identifier";

            case Token::Type::End:
                return os << "End";

            default:
                return os << "Unknown";
        }
    }
}
