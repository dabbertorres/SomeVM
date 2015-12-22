#include "Variable.hpp"

namespace dbr
{
	namespace sl
	{
		Variable::Variable(Type t)
		:	typeVal(t)
		{}

		Variable::Type Variable::type()
		{
			return typeVal;
		}
	}
}
