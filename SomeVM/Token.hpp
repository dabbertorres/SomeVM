#pragma once

#include <string>
#include <iosfwd>

namespace svm
{
	struct Token
	{
		enum class Type
		{
			// Literals
			Number,
			String,
			Bool,

			// symbols
			Crunch,
			Dollar,
			Semicolon,
            Colon,

            // Whitespace (for comments)
            Newline,

			Identifier,

            Unknown,
		};

		std::string value;
		Type type;
		size_t line;
		size_t character;

        Token(size_t line, size_t character);
        Token(std::string value, Type type, size_t line, size_t character);

        bool valid() const;
	};

    std::ostream& operator<<(std::ostream& os, const Token& t);
}
