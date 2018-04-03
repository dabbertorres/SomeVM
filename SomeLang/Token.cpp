#include "Token.hpp"

#include <ostream>

namespace sl
{
    Token::Token()
        : lexeme{},
        type{ Type::Unknown },
        line{ 0 },
        position{ 0 }
    {}

    Token::Token(size_t line, size_t position)
        : lexeme{},
        type{ Type::Unknown },
        line{ line },
        position{ position }
    {}

    Token::Token(std::string lexeme, Type type, size_t line, size_t position)
        : lexeme{ lexeme },
        type{ type },
        line{ line },
        position{ position }
    {}

    bool Token::valid() const
    {
        return type != Type::Unknown;
    }
}

std::ostream& operator<<(std::ostream& os, const sl::Token& t)
{
    os << "Token:\n"
        << "{\n"
        << "\tvalue:     " << t.lexeme << '\n'
        << "\ttype:      " << t.type << '\n'
        << "\tline:      " << t.line << '\n'
        << "\tcharacter: " << t.position << '\n'
        << "}\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, sl::Token::Type t)
{
    switch (t)
    {
    case sl::Token::Type::Float:
        return os << "Float";

    case sl::Token::Type::Int:
        return os << "Int";

    case sl::Token::Type::String:
        return os << "String";

    case sl::Token::Type::Bool:
        return os << "Bool";

    case sl::Token::Type::Semicolon:
        return os << "Semicolon";

    case sl::Token::Type::Colon:
        return os << "Colon";

    case sl::Token::Type::Comma:
        return os << "Comma";

    case sl::Token::Type::ParenOpen:
        return os << "ParenOpen";

    case sl::Token::Type::ParenClose:
        return os << "ParenClose";

    case sl::Token::Type::Newline:
        return os << "Newline";

    case sl::Token::Type::Dollar:
        return os << "Dollar";

    case sl::Token::Type::Arrow:
        return os << "Arrow";

    case sl::Token::Type::Func:
        return os << "Func";

    case sl::Token::Type::If:
        return os << "If";

    case sl::Token::Type::Switch:
        return os << "Switch";

    case sl::Token::Type::Case:
        return os << "Class";

    case sl::Token::Type::Else:
        return os << "Else";

    case sl::Token::Type::While:
        return os << "While";

    case sl::Token::Type::Identifier:
        return os << "Identifier";

    default:
        return os << "Unknown";
    }
}
