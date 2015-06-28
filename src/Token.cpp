#include "Token.hpp"

namespace lng
{
	Token::Token(Type t, const std::string& v)
	:	type(t),
		value(v)
	{}
}
