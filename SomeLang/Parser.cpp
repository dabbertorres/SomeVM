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

    static ParseLevel parseProgram(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseStatement(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseInstruction(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseIdentifier(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseValue(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseLiteral(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseVariable(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditional(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditionalIf(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditionalSwitch(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditionalCase(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseConditionalElse(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseLoop(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseFunction(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);
    static ParseLevel parseVariableList(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it);

    std::vector<Statement> parse(const std::string& stream, const std::vector<Token>& tokens)
    {
        std::stack<ParseLevel> state;
        state.push(ParseLevel::Program);

        std::vector<Statement> ret;

        std::stringstream line;

        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            line << it->value << ' ';

            try
            {
                switch (state.top())
                {
                    case ParseLevel::Program:
                    {
                        auto st = parseProgram(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Statement:
                    {
                        auto st = parseStatement(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Instruction:
                    {
                        auto st = parseInstruction(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Identifier:
                    {
                        auto st = parseIdentifier(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Value:
                    {
                        auto st = parseValue(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Literal:
                    {
                        auto st = parseLiteral(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Variable:
                    {
                        auto st = parseVariable(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Conditional:
                    {
                        auto st = parseConditional(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::ConditionalIf:
                    {
                        auto st = parseConditionalIf(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::ConditionalSwitch:
                    {
                        auto st = parseConditionalSwitch(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::ConditionalCase:
                    {
                        auto st = parseConditionalCase(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::ConditionalElse:
                    {
                        auto st = parseConditionalElse(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Loop:
                    {
                        auto st = parseLoop(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::Function:
                    {
                        auto st = parseFunction(stream, line.str(), it);
                        state.push(st);
                        break;
                    }

                    case ParseLevel::VariableList:
                    {
                        auto st = parseVariableList(stream, line.str(), it);
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

    ParseLevel parseProgram(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        if (startsStatement(*it))
            return ParseLevel::Statement;

        if (startsFunction(*it))
            return ParseLevel::Function;

        throw ParseError(stream, *it, line, "start of statement or function");
    }

    ParseLevel parseStatement(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        if (startsInstruction(*it))
            return ParseLevel::Instruction;

        if (startsConditional(*it))
            return ParseLevel::Conditional;

        if (startsLoop(*it))
            return ParseLevel::Loop;

        throw ParseError(stream, *it, line, "start of instruction, conditional, or loop");
    }

    ParseLevel parseInstruction(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseIdentifier(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseValue(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseLiteral(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseVariable(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditional(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditionalIf(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditionalSwitch(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditionalCase(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseConditionalElse(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseLoop(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseFunction(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseLevel parseVariableList(const std::string& stream, const std::string& line, std::vector<Token>::const_iterator it)
    {
        return ParseLevel::Program;
    }

    ParseError::ParseError(const std::string& stream, const Token& tok, const std::string& context, const std::string& expected)
    {
        std::ostringstream ss;
        std::string pointer(tok.position + 1, ' ');
        pointer[tok.position] = '^';

        ss << stream << "(" << tok.line << "," << tok.position << "): ERROR: Expected " << expected << ", got: " << tok.type << '\n'
            << context << '\n'
            << pointer << '\n';

        error = ss.str();
    }

    const char* ParseError::what() const
    {
        return error.c_str();
    }
}