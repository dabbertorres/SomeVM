#include "Repl.hpp"

namespace svm
{
	Repl::Repl(std::istream& input, std::ostream& output)
		: input{ input },
		output{ output }
	{}

	Repl::~Repl()
	{}

	void Repl::print(const Value& val) const
	{
		switch (val.type())
		{
		case Type::Nil:
			output << "Nil";
			break;

		case Type::Bool:
			output << std::boolalpha << static_cast<Bool>(val);
			break;

		case Type::Float:
			output << static_cast<Float>(val);
			break;

		default:
			break;
		}

		output << std::endl;
	}
}
