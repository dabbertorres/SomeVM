#include "Lexer.hpp"

#include <istream>
#include <sstream>

namespace
{
    svm::Token number(char first, std::istream& in, size_t line, size_t position);
    svm::Token string(char first, std::istream& in, size_t line, size_t position);
    svm::Token boolean(char first, std::istream& in, size_t line, size_t position);
    svm::Token identifier(char first, std::istream& in, size_t line, size_t position);
}

namespace svm
{
    std::vector<Token> lex(std::istream& in)
    {
        std::vector<Token> ret;

        size_t lineNumber = 1;
        size_t position = 0;

        char ch;
        while (in.get(ch))
        {
            ++position;

            switch (ch)
            {
            // number literal
            case '+':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                ret.push_back(number(ch, in, lineNumber, position));
                break;

            // start of string
            case '"':
                ret.push_back(string(ch, in, lineNumber, position));
                break;

            // possible start of boolean literal
            case 't':
            case 'f':
            {
                auto t = boolean(ch, in, lineNumber, position);
                if (t.valid())
                    ret.push_back(t);
                else
                    ret.push_back(identifier(ch, in, lineNumber, position));

                break;
            }

            // symbols
            case '#':
                ret.emplace_back("#", Token::Type::Crunch, lineNumber, position);
                break;

            case '$':
                ret.emplace_back("$", Token::Type::Dollar, lineNumber, position);
                break;

            case ';':
                ret.emplace_back(";", Token::Type::Semicolon, lineNumber, position);
                break;

            case ':':
                ret.emplace_back(":", Token::Type::Colon, lineNumber, position);
                break;

            // whitespace
            case '\n':
                ++lineNumber;
                position = 0;
                ret.emplace_back("", Token::Type::Newline, lineNumber, position);
                break;

            case '\r':
            case '\t':
            case ' ':
                break;

            // if it's not one of the above, it's likely an identifier
            default:
                ret.push_back(identifier(ch, in, lineNumber, position));
                break;
            }
        }

        return ret;
    }
}

namespace
{
    svm::Token number(char first, std::istream& in, size_t line, size_t position)
    {
        svm::Token ret{ line, position };

        bool hasPeriod = first == '.';

        auto start = in.tellg();
        bool done = false;
        char ch;

        while (!done && in.get(ch))
        {
            switch (ch)
            {
            case '.':
                if (!hasPeriod)
                {
                    hasPeriod = true;
                }
                else
                {
                    done = true;
                    in.unget();
                }
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                // don't need to do anything!
                break;

            default:
                done = true;
                in.unget();
                break;
            }
        }

        auto end = in.tellg();

        auto length = end - start;

        // 'start' is actually the character after 'first'
        ret.value.resize(length + 1);

        ret.value[0] = first;
        in.seekg(start);
        in.read(&ret.value[1], length);

        ret.type = svm::Token::Type::Number;

        return ret;
    }

    svm::Token string(char first, std::istream& in, size_t line, size_t position)
    {
        svm::Token ret{ line, position };

        auto start = in.tellg();

        // simply loop until we find the close quote
        char ch;
        while (in.get(ch) && ch != '"');

        auto end = in.tellg();
        auto length = end - start;

        // 'start' is actually the character after 'first'
        ret.value.resize(length + 1);
        ret.value[0] = first;

        in.seekg(start);
        in.read(&ret.value[1], length);

        ret.type = svm::Token::Type::String;

        return ret;
    }

    svm::Token boolean(char first, std::istream& in, size_t line, size_t position)
    {
        svm::Token ret{ line, position };

        if (first == 't')
        {
            char rue[3] = { '\0' };
            in.read(rue, sizeof(rue));

            if (std::strncmp(rue, "rue", sizeof(rue)) == 0)
            {
                ret.value = "true";
                ret.type = svm::Token::Type::Bool;
            }
            else
            {
                in.seekg(-4, std::ios::cur);
            }
        }
        else
        {
            char alse[4] = { '\0' };
            in.read(alse, sizeof(alse));

            if (std::strncmp(alse, "alse", sizeof(alse)) == 0)
            {
                ret.value = "false";
                ret.type = svm::Token::Type::Bool;
            }
            else
            {
                in.seekg(-5, std::ios::cur);
            }
        }

        return ret;
    }

    bool alpha(char ch)
    {
        return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z';
    }

    bool validIdentifier(char ch)
    {
        return alpha(ch) || '0' <= ch && ch <= '9' || ch == '_'; 
    }

    svm::Token identifier(char first, std::istream& in, size_t line, size_t position)
    {
        svm::Token ret{ line, position };

        if (alpha(first))
        {
            auto start = in.tellg();

            // greedy grab
            char ch;
            while (in.get(ch) && validIdentifier(ch));

            // remove trailing failed character
            in.unget();

            auto end = in.tellg();
            auto length = end - start;

            // 'start' is the character after 'first'
            ret.value.resize(length + 1);
            ret.value[0] = first;
            in.seekg(start);
            in.read(&ret.value[1], length);

            ret.type = svm::Token::Type::Identifier;
        }

        return ret;
    }
}
