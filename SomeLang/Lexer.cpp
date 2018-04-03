#include "Lexer.hpp"

#include <cstring>
#include <istream>
#include <sstream>
#include <string>

namespace sl
{
    static Token number(char first, std::istream& in, size_t line, size_t position);
    static Token string(char first, std::istream& in, size_t line, size_t position);
    static Token identifier(char first, std::istream& in, size_t line, size_t position);

    static Token keywordOrIdentifier(char first, std::istream& in, size_t line, size_t position);

    std::vector<Token> lex(std::istream& in)
    {
        if (!in)
            throw std::runtime_error("Unable to read input stream");

        std::vector<Token> ret;

        size_t lineNumber = 1;
        size_t position = 0;

        //std::streampos lineStart = in.tellg();
        //std::streampos lineEnd = lineStart;

        char ch;
        while (in.get(ch))
        {
            ++position;
            Token t;

            switch (ch)
            {
            // number literal
            case '-':
            case '+':
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
                position += ret.back().lexeme.length();
                break;

            // start of string
            case '"':
                ret.push_back(string(ch, in, lineNumber, position));
                position += ret.back().lexeme.length();
                break;

            case '#':
                // for comments, skip until we find a newline
                while (in.get(ch) && ch != '\n');
                ++lineNumber;
                position = 0;
                break;

            case '$':
                ret.emplace_back("$", Token::Type::Dollar, lineNumber, position);
                //ret.push_back(variable(ch, in, lineNumber, position));
                break;

            case '>':
                ret.emplace_back(">", Token::Type::Arrow, lineNumber, position);
                break;

            case ';':
                ret.emplace_back(";", Token::Type::Semicolon, lineNumber, position);
                break;

            case ':':
                ret.emplace_back(":", Token::Type::Colon, lineNumber, position);
                break;

            case ',':
                ret.emplace_back(",", Token::Type::Comma, lineNumber, position);
                break;

            case '(':
                ret.emplace_back("(", Token::Type::ParenOpen, lineNumber, position);
                break;

            case ')':
                ret.emplace_back(")", Token::Type::ParenClose, lineNumber, position);
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

            // if it's not one of the above, it's a keyword or an identifier
            default:
                ret.push_back(keywordOrIdentifier(ch, in, lineNumber, position));
                position += ret.back().lexeme.length();
                break;
            }
        }

        return ret;
    }

    Token number(char first, std::istream& in, size_t line, size_t position)
    {
        Token ret{ line, position };

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
        ret.lexeme.resize(length + 1);

        ret.lexeme[0] = first;
        in.seekg(start);
        in.read(&ret.lexeme[1], length);

        ret.type = hasPeriod ? Token::Type::Float : Token::Type::Int;

        return ret;
    }

    Token string(char first, std::istream& in, size_t line, size_t position)
    {
        Token ret{ line, position };

        auto start = in.tellg();

        // simply loop until we find the close quote
        char ch;
        while (in.get(ch) && ch != '"');

        auto end = in.tellg();
        auto length = end - start;

        // 'start' is actually the character after 'first'
        ret.lexeme.resize(length + 1);
        ret.lexeme[0] = first;

        in.seekg(start);
        in.read(&ret.lexeme[1], length);

        ret.type = Token::Type::String;

        return ret;
    }

    bool alpha(char ch)
    {
        return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
    }

    bool validIdentifier(char ch)
    {
        return alpha(ch) || ('0' <= ch && ch <= '9') || ch == '_';
    }

    Token identifier(char first, std::istream& in, size_t line, size_t position)
    {
        Token ret{ line, position };

        if (alpha(first))
        {
            auto start = in.tellg();

            // greedy grab
            char ch = 0;
            size_t adv = 1;
            while (in.get(ch) && validIdentifier(ch))
                ++adv;

            // remove trailing failed character
            if (adv > 1)
                in.unget();

            // 'start' is the character after 'first'
            ret.lexeme.resize(adv);
            ret.lexeme[0] = first;
            in.seekg(start);
            in.read(&ret.lexeme[1], adv - 1);

            ret.type = Token::Type::Identifier;
        }

        return ret;
    }

    static Token keywordOrIdentifier(char first, std::istream& in, size_t line, size_t position)
    {
        Token ret = identifier(first, in, line, position);

        // check for keywords
        if (ret.lexeme == "true" || ret.lexeme == "false")
        {
            ret.type = Token::Type::Bool;
        }
        else if (ret.lexeme == "func")
        {
            ret.type = Token::Type::Func;
        }
        else if (ret.lexeme == "if")
        {
            ret.type = Token::Type::If;
        }
        else if (ret.lexeme == "switch")
        {
            ret.type = Token::Type::Switch;
        }
        else if (ret.lexeme == "case")
        {
            ret.type = Token::Type::Case;
        }
        else if (ret.lexeme == "else")
        {
            ret.type = Token::Type::Else;
        }
        else if (ret.lexeme == "while")
        {
            ret.type = Token::Type::While;
        }

        return ret;
    }
}
