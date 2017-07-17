#include "Token.hpp"

#include <ostream>

static const char* TokenString(svm::Token::Type t)
{
    switch (t)
    {
    case svm::Token::Type::Number:
        return "Number";

    case svm::Token::Type::String:
        return "String";

    case svm::Token::Type::Bool:
        return "Bool";

    case svm::Token::Type::Crunch:
        return "Crunch";

    case svm::Token::Type::Dollar:
        return "Dollar";

    case svm::Token::Type::Semicolon:
        return "Semicolon";

    case svm::Token::Type::Colon:
        return "Colon";

    case svm::Token::Type::Newline:
        return "Newline";

    case svm::Token::Type::Identifier:
        return "Identifier";

    default:
        return "Unknown";
    }
}
namespace svm
{
    Token::Token(size_t line, size_t character)
        : value{},
        type{ Type::Unknown },
        line{ line },
        character{ character }
    {}

    Token::Token(std::string value, Type type, size_t line, size_t character)
        : value{ value },
        type{ type },
        line{ line },
        character{ character }
    {}

    bool Token::valid() const
    {
        return type != Type::Unknown;
    }

    std::ostream& operator<<(std::ostream& os, const Token& t)
    {
        os << "Token:\n"
            << "{\n"
            << "\tvalue:     '" << t.value << "'\n"
            << "\ttype:      " << TokenString(t.type) << '\n'
            << "\tline:      " << t.line << '\n'
            << "\tcharacter: " << t.character << '\n'
            << "}\n";
        return os;
    }
}
