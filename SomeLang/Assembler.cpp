#include "Assembler.hpp"

#include <istream>
#include <ostream>
#include <sstream>

#include "libSomeVM/Value.hpp"
#include "libSomeVM/Function.hpp"
#include "libSomeVM/Program.hpp"
#include "libSomeVM/VM.hpp"

#include "Util.hpp"

namespace
{
    using namespace sl;

    // returns the index of the added value
    static svm::Register constant(std::istream& in, std::vector<svm::Value>& constants)
    {
        std::string str;
        in >> str;

        bool isStr = false;

        // if a string, there may be spaces within the quotes, so make sure we get all of it
        if (str.front() == '"')
        {
            isStr = true;

            while (str.back() != '"')
            {
                std::string temp;
                in >> temp;
                str += temp;
            }
        }

        // string value
        if (isStr)
        {
            auto noquotes = str.substr(1, str.length() - 2);
            svm::String val{ noquotes.data(), noquotes.length() };
            constants.emplace_back(val);
        }
        else
        {
            util::toLower(str);

            if (util::isBool(str))
            {
                constants.emplace_back(util::strToBool(str));
            }
            else if (util::isNum(str))
            {
                constants.emplace_back(std::stod(str));
            }
            else
            {
                std::string msg = "Unrecognizable value \"";
                msg += str;
                msg += '"';

                throw std::runtime_error(msg);
            }
        }

        return static_cast<svm::Register>(constants.size() - 1);
    }

    static svm::Instruction oneArg(std::istream& in, svm::Instruction::Code type)
    {
        std::string str;

        in >> str;

        auto one = Assembler::toRegister(str);

        return{ type, one, 0, 0 };
    }

    static svm::Instruction oneArgConst(std::istream& in, svm::Instruction::Code type, svm::Program& prog)
    {
        std::string str;

        auto pos = in.tellg();
        in >> str;

        if (Assembler::isRegister(str))
        {
            return{ type, Assembler::toRegister(str), 0, 0 };
        }
        else
        {
            // undo stream extraction
            in.seekg(pos);
            auto idx = constant(in, prog.constants);

            return{ type, idx, 0, 0 };
        }
    }

    static svm::Instruction twoArg(std::istream& in, svm::Instruction::Code type)
    {
        std::string oneStr;
        std::string twoStr;

        in >> oneStr >> twoStr;

        auto one = Assembler::toRegister(oneStr);
        auto two = Assembler::toRegister(twoStr);

        return{ type, one, two, 0 };
    }

    static svm::Instruction twoArgOptConst(std::istream& in, svm::Instruction::Code type, svm::Instruction::Code constType, svm::Program& prog)
    {
        std::string one;
        std::string two;

        in >> one;

        auto pos = in.tellg();
        in >> two;

        auto dest = Assembler::toRegister(one);

        if (Assembler::isRegister(two))
        {
            return{ type, dest, Assembler::toRegister(two), 0 };
        }
        else
        {
            // undo extraction of two
            in.seekg(pos);
            auto idx = constant(in, prog.constants);

            return{ constType, dest, idx, 0 };
        }
    }

    static svm::Instruction threeArg(std::istream& in, svm::Instruction::Code type)
    {
        std::string oneStr;
        std::string twoStr;
        std::string threeStr;

        in >> oneStr >> twoStr >> threeStr;

        auto one = Assembler::toRegister(oneStr);
        auto two = Assembler::toRegister(twoStr);
        auto three = Assembler::toRegister(threeStr);

        return{ type, one, two, three };
    }
}

namespace sl
{
    svm::Program assemble(const std::vector<Statement>& stmts)
    {
        return {};
    }

    svm::Program Assembler::run(std::istream& in, std::ostream& logStream)
    {
        svm::Program program;

        run(in, logStream, program);

        return program;
    }

    void Assembler::run(std::istream& in, std::ostream& logStream, svm::Program& program)
    {
        std::uint64_t lineNum = 1;
        std::string line;

        std::stack<svm::Function> codeStack;

        // top level function has no returns and 1 argument (an array of command-line parameters) (or, will at least)
        codeStack.emplace(0, 1, svm::Bytecode{});

        for (; std::getline(in, line); ++lineNum)
        {
            if (line.empty())
                continue;

            std::string command;

            std::istringstream iss(line);
            iss >> command;

            if (command.empty())
                continue;

            util::toLower(command);

            try
            {
                auto it = commands.end();

                if (codeStack.empty())
                    throw std::runtime_error("Unexpected end of function");

                auto& top = codeStack.top();

                // comment
                if (command[0] == '#')
                {
                    continue;
                }
                // const declaration
                else if (command == "const")
                {
                    constant(iss, program.constants);
                }
                // command
                else if ((it = commands.find(command)) != commands.end())
                {
                    auto inst = it->second(iss, program);
                    //top.code.push_back(inst);
                }
                // start function
                else if (command.back() == ':')
                {
                    // number of returns is the first number following the colon
                    // number of arguments is the second number following the colon (with a space between them! (strict for now...))
                    std::uint32_t numRets = 0;
                    std::uint32_t nargs = 0;
                    iss >> numRets >> nargs;

                    codeStack.emplace(static_cast<std::uint8_t>(numRets), static_cast<std::uint8_t>(nargs), svm::Bytecode{});
                }
                // end function
                else if (command == "end")
                {
                    program.functions.push_back(top);
                    codeStack.pop();
                }
                else
                {
                    std::stringstream ss;
                    ss << "Unknown command \"" << command << "\"";
                    throw std::runtime_error(ss.str());

                }
            }
            catch (const std::exception& e)
            {
                logStream << "\nError (line: " << lineNum << "): " << e.what() << std::endl;
                throw e;
            }
        }

        // if the top-level function was not popped
        if (!codeStack.empty())
        {
            program.functions.insert(program.functions.begin(), codeStack.top());
            codeStack.pop();
        }
    }

    bool Assembler::isRegister(const std::string& str)
    {
        if (str[0] != '$')
            return false;

        auto rest = str.substr(1);

        // check decimal
        if (util::isInt(rest))
            return true;

        if (util::isHex(rest))
            return true;

        return false;
    }

    svm::Register Assembler::toRegister(const std::string& regStr)
    {
        if (!isRegister(regStr))
        {
            std::string msg = "Expected register prefix '$', got \'";
            msg += regStr[0];
            msg += '\'';

            throw std::runtime_error(msg);
        }

        return static_cast<svm::Register>(std::stoi(regStr.substr(1)));
    }

    const std::unordered_map<std::string, svm::Instruction(*)(std::istream&, svm::Program&)> Assembler::commands =
    {
        /* memory ops */
        {"load", [](std::istream& in, svm::Program& prog) { return oneArgConst(in, svm::Instruction::Code::Load, prog); }},

        /* math ops */
        {"add", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Add); }},
        {"sub", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Sub); }},
        {"mult", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Mult); }},
        {"div", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Div); }},
        {"mod", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Mod); }},
        {"neg", [](std::istream& in, svm::Program&) { return twoArg(in, svm::Instruction::Code::Neg); }},

        /* comparison ops */
        {"lt", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Lt); }},
        {"lteq", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::LtEq); }},
        {"gt", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Gt); }},
        {"gteq", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::GtEq); }},
        {"eq", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Eq); }},
        {"neq", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Neq); }},

        /* logical ops */
        {"not", [](std::istream& in, svm::Program&) { return twoArg(in, svm::Instruction::Code::Not); }},
        {"and", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Add); }},
        {"or", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Or); }},
        {"xor", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Xor); }},

        /* conditional branching */
        {"jmpt", [](std::istream& in, svm::Program& prog) { return twoArg(in, svm::Instruction::Code::JmpT); }},
        {"jmpf", [](std::istream& in, svm::Program& prog) { return twoArg(in, svm::Instruction::Code::JmpF); }},

        /* branching */
        {"call", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::Call); }},
        {"ret", [](std::istream& in, svm::Program&) { return twoArg(in, svm::Instruction::Code::Ret); }},
        {"jmp", [](std::istream& in, svm::Program& prog) { return oneArgConst(in, svm::Instruction::Code::Jmp, prog); }},

        /* misc */
        {"syscall", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Code::SysCall); }},
        {"nop", [](std::istream&, svm::Program&) { return svm::Instruction(svm::Instruction::Code::Nop, 0, 0, 0); }},
    };
}
