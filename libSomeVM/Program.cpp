#include "Program.hpp"

#include <cstdint>
#include <cstring>
#include <istream>
#include <ostream>
#include <variant>

namespace svm
{
    constexpr auto* BINARY_ID = ".svm";
    constexpr uint32_t VERSION = 0;

    Program::Program()
        : constants{},
        functions{}
    {}

    uint64_t Program::load(std::istream& input)
    {
        if (!input)
            throw std::runtime_error("Unable to access input stream");

        auto startPos = input.tellg();

        std::string identifier{ 4, '\0' };
        input.read(&identifier[0], std::strlen(BINARY_ID));

        if (identifier != BINARY_ID)
            throw std::runtime_error("Input file is not a valid svm binary");

        // check version...
        uint32_t version = 0;
        input.read(reinterpret_cast<char*>(&version), sizeof(version));

        if (version != VERSION)
            throw std::runtime_error("Incompatible version");

        // constants
        uint64_t numConstants = 0;
        input.read(reinterpret_cast<char*>(&numConstants), sizeof(numConstants));

        for (uint64_t idx = 0; idx < numConstants; ++idx)
        {
            uint64_t type;
            input.read(reinterpret_cast<char*>(&type), sizeof(type));

            Bool b;
            Int i;
            Float f;
            std::vector<char> s;
            uint64_t len;

            switch (type)
            {
            case typeIndex<Bool>():
                input.read(reinterpret_cast<char*>(&b), sizeof(b));
                constants.emplace_back(b);
                break;

            case typeIndex<Int>():
                input.read(reinterpret_cast<char*>(&i), sizeof(i));
                constants.emplace_back(i);
                break;

            case typeIndex<Float>():
                input.read(reinterpret_cast<char*>(&f), sizeof(f));
                constants.emplace_back(f);
                break;

            case typeIndex<String>():
                input.read(reinterpret_cast<char*>(&len), sizeof(len));
                s.resize(len);
                input.read(s.data(), len);
                constants.emplace_back(String{ s.data(), len });
                break;

            default:
                throw std::runtime_error{ "invalid type for constant" };
            }
        }

        // functions
        uint64_t numFunctions = 0;
        input.read(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

        for (uint64_t i = 0; i < numFunctions; ++i)
        {
            uint8_t nrets;
            uint8_t nargs;

            input.read(reinterpret_cast<char*>(&nrets), sizeof(nrets));
            input.read(reinterpret_cast<char*>(&nargs), sizeof(nargs));

            uint64_t numInstrs;
            input.read(reinterpret_cast<char*>(&numInstrs), sizeof(numInstrs));

            Bytecode code(numInstrs);

            input.read(reinterpret_cast<char*>(code.data()), numInstrs * sizeof(Instruction));

            functions.emplace_back(nrets, nargs, code);
        }

        return input.tellg() - startPos;
    }

    uint64_t Program::write(std::ostream& output) const
    {
        if (!output)
            throw std::runtime_error("Unable to access output stream");

        auto startPos = output.tellp();

        // file type identifier
        output.write(BINARY_ID, std::strlen(BINARY_ID));

        // TODO: need to figure out how I want to do versioning
        // version
        uint32_t version = 0;
        output.write(reinterpret_cast<char*>(&version), sizeof(version));

        // constants
        uint64_t numConstants = constants.size();
        output.write(reinterpret_cast<char*>(&numConstants), sizeof(numConstants));

        for (auto c : constants)
        {
            uint64_t type = c.index();
            output.write(reinterpret_cast<char*>(&type), sizeof(type));

            std::visit([&](auto&& v)
                       {
                           using T = std::decay_t<decltype(v)>;

                           if constexpr (std::is_same_v<T, String>)
                           {
                               uint64_t len = v.length();
                               output.write(reinterpret_cast<char*>(&len), sizeof(len));
                               output.write(v.data(), len);
                           }
                           else
                           {
                               output.write(reinterpret_cast<char*>(&v), sizeof(T));
                           }
                       }, c);
        }

        // functions
        uint64_t numFunctions = functions.size();
        output.write(reinterpret_cast<char*>(&numFunctions), sizeof(numFunctions));

        for (auto& f : functions)
        {
            uint8_t nrets = f.returns();
            uint8_t nargs = f.args();

            output.write(reinterpret_cast<const char*>(&nrets), sizeof(nrets));
            output.write(reinterpret_cast<const char*>(&nargs), sizeof(nargs));

            uint64_t numInstrs = f.length();

            auto& code = f.bytecode();
            output.write(reinterpret_cast<const char*>(&numInstrs), sizeof(numInstrs));
            output.write(reinterpret_cast<const char*>(code.data()), numInstrs * sizeof(Instruction));
        }

        return output.tellp() - startPos;
    }
}

