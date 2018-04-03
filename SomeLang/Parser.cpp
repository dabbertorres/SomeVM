#include "Parser.hpp"

#include <istream>
#include <ostream>
#include <sstream>
#include <stack>

#include "libSomeVM/Instruction.hpp"
#include "libSomeVM/Register.hpp"
#include "libSomeVM/Value.hpp"

namespace sl
{
    class ParseError : public std::exception
    {
    public:
        ParseError(std::string error);
        ParseError(const std::string& streamName, std::istream& stream, Token token, std::string expected);

        const char* what() const noexcept override;

    private:
        std::string error;
    };

    class EndOfFileError : public ParseError
    {
    public:
        EndOfFileError(const std::string& streamName, const std::string& expect);
    };

    struct ParseState
    {
        std::vector<Token>::const_iterator end;
        const std::string& streamName;
        std::istream& inputStream;

        std::vector<Token>::const_iterator& it;
    };

    static void parseStatement(ParseState& state, Statement& ret);
    static void parseInstruction(ParseState& state, StatementInstruction& ret);
    static void parseValue(ParseState& state, Value& ret);
    static void parseLiteral(ParseState& state, svm::Value& ret);
    static void parseVariable(ParseState& state, Variable& ret);
    static void parseIf(ParseState& state, StatementIf& ret);
    static void parseElse(ParseState& state, std::vector<Statement>& ret);
    static void parseSwitch(ParseState& state, StatementSwitch& ret);
    static void parseCase(ParseState& state, StatementCase& ret);
    static void parseWhile(ParseState& state, StatementWhile& ret);
    static void parseFunction(ParseState& state, StatementFunction& ret);
    static void parseVariableList(ParseState& state, std::vector<Variable>& vars);

    static void consumeWhitespace(ParseState& state);

    static bool isValue(const Token& tok);
    static bool isLiteral(const Token& tok);
    static bool isSymbol(const Token& tok);

    static bool startsStatement(const Token& tok);
    static bool startsInstruction(const Token& tok);
    static bool startsVariable(const Token& tok);
    static bool startsIf(const Token& tok);
    static bool startsSwitch(const Token& tok);
    static bool startsWhile(const Token& tok);
    static bool startsFunction(const Token& tok);

    std::vector<Statement> parse(const std::string& streamName, std::istream& inputStream, std::ostream& logStream, const std::vector<Token>& tokens)
    {
        std::vector<Statement> ret;

        auto it = tokens.begin();
        ParseState state{ tokens.end(), streamName, inputStream, it };

        while (state.it != state.end)
        {
            try
            {
                ret.emplace_back();
                parseStatement(state, ret.back());
            }
            catch (const std::exception& e)
            {
                logStream << e.what() << std::endl;

                // now undo what failed, and try again at the next statement
                ret.pop_back();

                while (it->type != Token::Type::Newline)
                    ++it;
                // not exiting loop allows us to potentially resume parsing at the next best point
            }
        }

        ret.pop_back();

        return ret;
    }

    void parseStatement(ParseState& state, Statement& ret)
    {
        consumeWhitespace(state);

        if (state.it == state.end)
            return;

        if (startsIf(*state.it))
        {
            StatementIf i;
            parseIf(state, i);
            ret = i;
        }
        else if (startsSwitch(*state.it))
        {
            StatementSwitch s;
            parseSwitch(state, s);
            ret = s;
        }
        else if (startsWhile(*state.it))
        {
            StatementWhile w;
            parseWhile(state, w);
            ret = w;
        }
        else if (startsFunction(*state.it))
        {
            StatementFunction f;
            parseFunction(state, f);
            ret = f;
        }
        else if (startsInstruction(*state.it))
        {
            StatementInstruction i;
            parseInstruction(state, i);
            ret = i;
        }
        else
        {
            throw ParseError{ state.streamName, state.inputStream, *state.it, "instruction, if, switch, or while" };
        }
    }

    void parseInstruction(ParseState& state, StatementInstruction& ret)
    {
        ret.instruction = state.it->lexeme;
        ++state.it;

        while (isValue(*state.it))
        {
            ret.args.emplace_back();
            parseValue(state, ret.args.back());
        }

        if (state.it->type != Token::Type::Newline && state.it->type != Token::Type::Semicolon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "value, newline, or semicolon" };
    }

    void parseValue(ParseState& state, Value& ret)
    {
        if (isLiteral(*state.it))
        {
            svm::Value v;
            parseLiteral(state, v);
            ret = v;
        }
        else if (state.it->type == Token::Type::Dollar)
        {
            Variable v;
            parseVariable(state, v);
            ret = v;
        }
        else
        {
            throw ParseError{ state.streamName, state.inputStream, *state.it, "value or variable" };
        }
    }

    void parseLiteral(ParseState& state, svm::Value& ret)
    {
        switch (state.it->type)
        {
        case Token::Type::Float:
            ret = std::stod(state.it->lexeme);
            break;

        case Token::Type::Int:
            ret = std::stoll(state.it->lexeme);
            break;

        case Token::Type::String:
            ret = svm::String{ state.it->lexeme.data(), state.it->lexeme.length() };
            break;

        case Token::Type::Bool:
            ret = state.it->lexeme == "true";
            break;

        default:
            throw ParseError{ state.streamName, state.inputStream, *state.it, "float, int, string, or bool" };
        }

        ++state.it;
    }

    void parseVariable(ParseState& state, Variable& ret)
    {
        if (state.it->type != Token::Type::Dollar)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "$" };
        ++state.it;

        if (state.it->type != Token::Type::Identifier)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "identifier" };

        ret.identifier = state.it->lexeme;
        ++state.it;
    }

    void parseIf(ParseState& state, StatementIf& ret)
    {
        if (state.it->type != Token::Type::If)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "if" };
        ++state.it;

        parseValue(state, ret.condition);

        if (state.it->type != Token::Type::Colon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ":" };
        ++state.it;

        consumeWhitespace(state);

        while (startsStatement(*state.it))
        {
            ret.trueBranch.emplace_back();
            parseStatement(state, ret.trueBranch.back());
        }

        consumeWhitespace(state);

        if (state.it->type == Token::Type::Else)
            parseElse(state, ret.falseBranch);

        consumeWhitespace(state);

        if (state.it->type != Token::Type::Semicolon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ";" };
        ++state.it;
    }

    void parseSwitch(ParseState& state, StatementSwitch& ret)
    {
        if (state.it->type != Token::Type::Switch)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "switch" };
        ++state.it;

        parseValue(state, ret.condition);

        if (state.it->type != Token::Type::Colon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ":" };
        ++state.it;

        while (state.it->type == Token::Type::Case)
        {
            ret.cases.emplace_back();
            parseCase(state, ret.cases.back());
        }

        if (state.it->type == Token::Type::Else)
            parseElse(state, ret.elseStatements);

        if (state.it->type != Token::Type::Semicolon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ";" };
    }

    void parseCase(ParseState& state, StatementCase& ret)
    {
        if (state.it->type != Token::Type::Case)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "case" };
        ++state.it;

        parseValue(state, ret.condition);

        if (state.it->type != Token::Type::Colon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ":" };
        ++state.it;

        while (startsStatement(*state.it))
        {
            ret.statements.emplace_back();
            parseStatement(state, ret.statements.back());
        }

        if (state.it->type != Token::Type::Semicolon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ";" };
        ++state.it;
    }

    void parseElse(ParseState& state, std::vector<Statement>& ret)
    {
        if (state.it->type != Token::Type::Else)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "else" };
        ++state.it;

        if (state.it->type != Token::Type::Colon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ":" };
        ++state.it;

        consumeWhitespace(state);

        while (startsStatement(*state.it))
        {
            ret.emplace_back();
            parseStatement(state, ret.back());
        }
    }

    void parseWhile(ParseState& state, StatementWhile& ret)
    {
        if (state.it->type != Token::Type::While)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "while" };
        ++state.it;

        parseValue(state, ret.condition);

        if (state.it->type != Token::Type::Colon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ":" };
        ++state.it;

        while (startsStatement(*state.it))
        {
            ret.statements.emplace_back();
            parseStatement(state, ret.statements.back());
        }

        if (state.it->type != Token::Type::Semicolon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ";" };
        ++state.it;
    }

    void parseFunction(ParseState& state, StatementFunction& ret)
    {
        if (state.it->type != Token::Type::Func)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "func" };
        ++state.it;

        if (state.it->type != Token::Type::Identifier)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "identifier" };

        ret.identifier = state.it->lexeme;
        ++state.it;

        parseVariableList(state, ret.args);

        if (state.it->type != Token::Type::Arrow)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ">" };
        ++state.it;

        parseVariableList(state, ret.rets);

        if (state.it->type != Token::Type::Colon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ":" };
        ++state.it;

        while (startsStatement(*state.it))
        {
            ret.statements.emplace_back();
            parseStatement(state, ret.statements.back());
        }

        if (state.it->type != Token::Type::Semicolon)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ";" };
        ++state.it;
    }

    void parseVariableList(ParseState& state, std::vector<Variable>& ret)
    {
        if (state.it->type != Token::Type::ParenOpen)
            throw ParseError{ state.streamName, state.inputStream, *state.it, "(" };
        ++state.it;

        while (state.it->type == Token::Type::Comma)
        {
            ++state.it;
            ret.emplace_back();
            parseVariable(state, ret.back());
        }

        if (state.it->type != Token::Type::ParenClose)
            throw ParseError{ state.streamName, state.inputStream, *state.it, ")" };
        ++state.it;
    }

    void consumeWhitespace(ParseState& state)
    {
        while (state.it != state.end && state.it->type == Token::Type::Newline)
            ++state.it;
    }

    bool isValue(const Token& tok)
    {
        return isLiteral(tok) || startsVariable(tok);
    }

    bool isLiteral(const Token& tok)
    {
        switch (tok.type)
        {
        case Token::Type::Float:
        case Token::Type::Int:
        case Token::Type::String:
        case Token::Type::Bool:
            return true;

        default:
            return false;
        }
    }

    bool isSymbol(const Token& tok)
    {
        switch (tok.type)
        {
        case Token::Type::Semicolon:
        case Token::Type::Colon:
        case Token::Type::Comma:
        case Token::Type::ParenOpen:
        case Token::Type::ParenClose:
        case Token::Type::Newline:
        case Token::Type::Dollar:
        case Token::Type::Arrow:
            return true;

        default:
            return false;
        }
    }

    bool startsStatement(const Token& tok)
    {
        return startsInstruction(tok) || startsIf(tok) || startsSwitch(tok) || startsWhile(tok);
    }

    bool startsInstruction(const Token& tok)
    {
        return tok.type == Token::Type::Identifier;
    }

    bool startsVariable(const Token& tok)
    {
        return tok.type == Token::Type::Dollar;
    }

    bool startsConditional(const Token& tok)
    {
        return startsIf(tok) || startsSwitch(tok);
    }

    bool startsIf(const Token& tok)
    {
        return tok.type == Token::Type::If;
    }

    bool startsSwitch(const Token& tok)
    {
        return tok.type == Token::Type::Switch;
    }

    bool startsWhile(const Token& tok)
    {
        return tok.type == Token::Type::While;
    }

    bool startsFunction(const Token& tok)
    {
        return tok.type == Token::Type::Func;
    }

    ParseError::ParseError(std::string error)
        : error{ error }
    {}

    ParseError::ParseError(const std::string& streamName, std::istream& stream, Token token, std::string expected)
    {
        // get the line the error occured on for context
        stream.clear();
        stream.seekg(std::ios::beg);

        std::string line;
        size_t lineNum = 0;

        do
        {
            std::getline(stream, line);
            ++lineNum;
        }
        while (lineNum != token.line);

        // now format the error message
        std::ostringstream ss;
        std::string pointer(token.position, ' ');
        if (token.position > 0)
            pointer[token.position - 1] = '^';
        else
            pointer = " ";

        ss << "[ERROR] " << streamName << "(" << token.line << "," << token.position << "): Expected " << expected << ", got: " << token.type << '\n'
            << line << '\n'
            << pointer << '\n';
        error = ss.str();
    }

    const char* ParseError::what() const noexcept
    {
        return error.c_str();
    }

    EndOfFileError::EndOfFileError(const std::string& streamName, const std::string& expect)
        : ParseError{ "[ERROR] " + streamName + "(end of file): expected " + expect }
    {}
}

