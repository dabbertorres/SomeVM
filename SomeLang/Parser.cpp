#include "Parser.hpp"

#include <iostream>
#include <stack>

namespace sl
{
    enum class ParseLevel
    {
        Program,
        Statement,
        Instruction,
        Identifier,
        Value,
        Literal,
        Variable,
        Conditional,
        ConditionalIf,
        ConditionalSwitch,
        ConditionalCase,
        ConditionalElse,
        Loop,
        Function,
        VariableList,
    };

    static ParseLevel parseProgram(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseStatement(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseInstruction(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseIdentifier(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseValue(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseLiteral(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseVariable(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditional(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditionalIf(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditionalSwitch(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditionalCase(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditionalElse(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseLoop(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseFunction(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);
    static ParseLevel parseVariableList(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it);

    static bool isValue(Token tok);
    static bool isLiteral(Token tok);

    static bool startsStatement(Token tok);
    static bool startsInstruction(Token tok);
    static bool startsVariable(Token tok);
    static bool startsConditional(Token tok);
    static bool startsIf(Token tok);
    static bool startsSwitch(Token tok);
    static bool startsCase(Token tok);
    static bool startsElse(Token tok);
    static bool startsLoop(Token tok);
    static bool startsFunction(Token tok);
    static bool startsVariableList(Token tok);

    std::vector<Statement> parse(const std::string& streamName, std::istream& stream, const std::vector<Token>& tokens)
    {
        std::stack<ParseLevel> state;
        std::vector<Statement> ret;

        // get our starting point
        auto st = parseProgram(streamName, stream, tokens.begin());
        state.push(st);

        for (auto it = tokens.begin(); it != tokens.end(); )
        {
            try
            {
                switch (state.top())
                {
                    case ParseLevel::Statement:
                    {
                        auto st = parseStatement(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Instruction:
                    {
                        auto st = parseInstruction(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Identifier:
                    {
                        auto st = parseIdentifier(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Value:
                    {
                        auto st = parseValue(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Literal:
                    {
                        auto st = parseLiteral(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Variable:
                    {
                        auto st = parseVariable(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Conditional:
                    {
                        auto st = parseConditional(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::ConditionalIf:
                    {
                        auto st = parseConditionalIf(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::ConditionalSwitch:
                    {
                        auto st = parseConditionalSwitch(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::ConditionalCase:
                    {
                        auto st = parseConditionalCase(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::ConditionalElse:
                    {
                        auto st = parseConditionalElse(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Loop:
                    {
                        auto st = parseLoop(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Function:
                    {
                        auto st = parseFunction(streamName, stream, it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::VariableList:
                    {
                        auto st = parseVariableList(streamName, stream, it);
                        state.push(st);
                        break;
                    }
                }
            }
            catch (const ParseError& pe)
            {
                std::cout << pe.what() << std::flush;
                // TODO attempt to recover
            }
        }

        return ret;
    }

    ParseLevel parseProgram(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        if (startsStatement(*it))
            return ParseLevel::Statement;

        if (startsFunction(*it))
            return ParseLevel::Function;

        throw ParseError(streamName, *it, stream, "start of statement or function");
    }

    ParseLevel parseStatement(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        if (startsInstruction(*it))
            return ParseLevel::Instruction;

        if (startsConditional(*it))
            return ParseLevel::Conditional;

        if (startsLoop(*it))
            return ParseLevel::Loop;

        throw ParseError(streamName, *it, stream, "start of instruction, conditional, or loop");
    }

    ParseLevel parseInstruction(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseIdentifier(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseValue(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseLiteral(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseVariable(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditional(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditionalIf(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditionalSwitch(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditionalCase(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditionalElse(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseLoop(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseFunction(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseVariableList(const std::string& streamName, std::istream& stream, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    bool isValue(Token tok)
    {
        return isLiteral(tok) || startsVariable(tok);
    }

    bool isLiteral(Token tok)
    {
        switch (tok.type)
        {
            case Token::Type::Number:
            case Token::Type::String:
            case Token::Type::Bool:
                return true;

            default:
                return false;
        }
    }

    bool startsStatement(Token tok)
    {
        return startsInstruction(tok) || startsConditional(tok) || startsLoop(tok);
    }

    bool startsInstruction(Token tok)
    {
        return tok.type == Token::Type::Identifier;
    }

    bool startsVariable(Token tok)
    {
        return tok.type == Token::Type::Dollar;
    }

    bool startsConditional(Token tok)
    {
        return startsIf(tok) || startsSwitch(tok);
    }

    bool startsIf(Token tok)
    {
        return tok.type == Token::Type::If;
    }

    bool startsSwitch(Token tok)
    {
        return tok.type == Token::Type::Switch;
    }

    bool startsCase(Token tok)
    {
        return tok.type == Token::Type::Case;
    }

    bool startsElse(Token tok)
    {
        return tok.type == Token::Type::Else;
    }

    bool startsLoop(Token tok)
    {
        return tok.type == Token::Type::While;
    }

    bool startsFunction(Token tok)
    {
        return tok.type == Token::Type::Func;
    }

    bool startsVariableList(Token tok)
    {
        return tok.type == Token::Type::ParenLeft;
    }

    ParseError::ParseError(const std::string& streamName, const Token& tok, std::istream& stream, const std::string& expected)
    {
        std::ostringstream ss;
        std::string pointer(tok.position, ' ');
        pointer[tok.position - 1] = '^';

        ss << "[ERROR] " << streamName << "(" << tok.line << "," << tok.position << "): Expected " << expected << ", got: " << tok.type << '\n'
            << getLine(tok, stream) << '\n'
            << pointer << '\n';

        error = ss.str();
    }

    const char* ParseError::what() const
    {
        return error.c_str();
    }

    std::string ParseError::getLine(const Token& tok, std::istream& stream)
    {
        stream.clear();
        stream.seekg(std::ios::beg);

        std::string line;
        size_t lineNum = 0;

        do
        {
            std::getline(stream, line);
            ++lineNum;
        }
        while (lineNum != tok.line);

        return line;
    }
}