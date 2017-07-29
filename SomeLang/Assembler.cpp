#include "Assembler.hpp"

#include <string>
#include <sstream>
#include <istream>
#include <ostream>

#include "libSomeVM/Value.hpp"
#include "libSomeVM/Function.hpp"
#include "libSomeVM/Program.hpp"
#include "libSomeVM/VM.hpp"

#include "Util.hpp"

namespace
{
    using namespace sl;

    // returns the index of the added value
    static std::uint64_t constant(std::istream& in, svm::Registry& constants)
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
            svm::Array<char> val{ noquotes.data(), noquotes.length() };
            constants.push_back(val);
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

        return constants.size() - 1;
    }

    static svm::Instruction oneArg(std::istream& in, svm::Instruction::Type type)
    {
        std::string str;

        in >> str;

        auto one = static_cast<std::uint64_t>(Assembler::toRegister(str));

        return{ type, one };
    }

    static svm::Instruction oneArgConst(std::istream& in, svm::Instruction::Type type, svm::Instruction::Type constType, svm::Program& prog)
    {
        std::string str;

        auto pos = in.tellg();
        in >> str;

        if (Assembler::isRegister(str))
        {
            return{ type, Assembler::toRegister(str) };
        }
        else
        {
            // undo stream extraction
            in.seekg(pos);
            auto idx = constant(in, prog.constants);

            return{ constType, idx };
        }
    }

    static svm::Instruction twoArg(std::istream& in, svm::Instruction::Type type)
    {
        std::string oneStr;
        std::string twoStr;

        in >> oneStr >> twoStr;

        std::uint32_t one = Assembler::toRegister(oneStr);
        std::uint32_t two = Assembler::toRegister(twoStr);

        return{ type, one, two };
    }

    static svm::Instruction twoArgOptConst(std::istream& in, svm::Instruction::Type type, svm::Instruction::Type constType, svm::Program& prog)
    {
        std::string one;
        std::string two;

        in >> one;

        auto pos = in.tellg();
        in >> two;

        auto dest = Assembler::toRegister(one);

        if (Assembler::isRegister(two))
        {
            return{ type, dest, Assembler::toRegister(two) };
        }
        else
        {
            // undo extraction of two
            in.seekg(pos);
            auto idx = constant(in, prog.constants);

            return{ constType, dest, static_cast<std::uint32_t>(idx) };
        }
    }

    static svm::Instruction threeArg(std::istream& in, svm::Instruction::Type type)
    {
        std::string oneStr;
        std::string twoStr;
        std::string threeStr;

        in >> oneStr >> twoStr >> threeStr;

        auto one = static_cast<std::uint16_t>(Assembler::toRegister(oneStr));
        auto two = static_cast<std::uint16_t>(Assembler::toRegister(twoStr));
        auto three = static_cast<std::uint16_t>(Assembler::toRegister(threeStr));

        return{ type, one, two, three };
    }
}

namespace sl
{
    svm::Program assemble(const std::vector<Statement>& stmts)
    {
        return {};
    }

    svm::Program Assembler::run(std::istream& in, std::ostream& out)
    {
        svm::Program program;

        run(in, out, program);

        return program;
    }

    void Assembler::run(std::istream& in, std::ostream& out, svm::Program& program)
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
                    std::uint32_t numArgs = 0;
                    iss >> numRets >> numArgs;

                    codeStack.emplace(static_cast<std::uint8_t>(numRets), static_cast<std::uint8_t>(numArgs), svm::Bytecode{});
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
                out << "\nError (line: " << lineNum << "): " << e.what() << std::endl;
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

    std::uint16_t Assembler::toRegister(const std::string& regStr)
    {
        if (!isRegister(regStr))
        {
            std::string msg = "Expected register prefix '$', got \'";
            msg += regStr[0];
            msg += '\'';

            throw std::runtime_error(msg);
        }

        return static_cast<std::uint16_t>(std::stoi(regStr.substr(1)));
    }

    const std::unordered_map<std::string, svm::Instruction(*)(std::istream&, svm::Program&)> Assembler::commands =
	{
		/* memory ops */
		{"load", [](std::istream& in, svm::Program& prog) { return twoArgOptConst(in, svm::Instruction::Type::Load, svm::Instruction::Type::LoadC, prog); }},
		{"loadc", [](std::istream& in, svm::Program& prog) { return twoArgOptConst(in, svm::Instruction::Type::Load, svm::Instruction::Type::LoadC, prog); }},

		/* math ops */
		{"add", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Add); }},
		{"sub", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Sub); }},
		{"mult", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Mult); }},
		{"div", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Div); }},
		{"mod", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Mod); }},
		{"neg", [](std::istream& in, svm::Program&) { return twoArg(in, svm::Instruction::Type::Neg); }},

		/* comparison ops */
		{"lt", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Lt); }},
		{"lteq", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::LtEq); }},
		{"gt", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Gt); }},
		{"gteq", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::GtEq); }},
		{"eq", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Eq); }},
		{"neq", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Neq); }},

		/* logical ops */
		{"not", [](std::istream& in, svm::Program&) { return twoArg(in, svm::Instruction::Type::Not); }},
		{"and", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Add); }},
		{"or", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Or); }},
		{"xor", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Xor); }},

		/* conditional branching */
		{"jmpt", [](std::istream& in, svm::Program& prog) { return twoArgOptConst(in, svm::Instruction::Type::JmpT, svm::Instruction::Type::JmpTC, prog); }},
		{"jmpf", [](std::istream& in, svm::Program& prog) { return twoArgOptConst(in, svm::Instruction::Type::JmpF, svm::Instruction::Type::JmpFC, prog); }},

		{"rjmpt", [](std::istream& in, svm::Program& prog) { return twoArgOptConst(in, svm::Instruction::Type::RJmpT, svm::Instruction::Type::RJmpTC, prog); }},
		{"rjmpf", [](std::istream& in, svm::Program& prog) { return twoArgOptConst(in, svm::Instruction::Type::RJmpF, svm::Instruction::Type::RJmpFC, prog); }},

		/* branching */
		{"call", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::Call); }},
		{"ret", [](std::istream& in, svm::Program&) { return twoArg(in, svm::Instruction::Type::Ret); }},
		{"jmp", [](std::istream& in, svm::Program& prog) { return oneArgConst(in, svm::Instruction::Type::Jmp, svm::Instruction::Type::JmpC, prog); }},
		{"rjmp", [](std::istream& in, svm::Program& prog) { return oneArgConst(in, svm::Instruction::Type::RJmp, svm::Instruction::Type::RJmpC, prog); }},

		/* misc */
		{"syscall", [](std::istream& in, svm::Program&) { return threeArg(in, svm::Instruction::Type::SysCall); }},
		{"nop", [](std::istream&, svm::Program&) { return svm::Instruction(svm::Instruction::Type::Nop, 0); }},
	};
}
