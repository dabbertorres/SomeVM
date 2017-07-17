#pragma once

#include <stdexcept>
#include <sstream>

namespace svm
{
    class ParseError : public std::exception
    {
    public:
        ParseError(const std::string& stream, size_t line, size_t position, const std::string& context, const std::string& msg)
        {
            std::ostringstream ss;
            std::string pointer(position + 1, ' ');
            pointer[position] = '^';

            ss << stream << "(" << line << "," << position << "): ERROR: " << msg << '\n'
                << context << '\n'
                << pointer << '\n';

            error = ss.str();
        }

        ~ParseError() = default;

        const char* what() const override
        {
            return error.c_str();
        }

    private:
        std::string error;
    };
}
