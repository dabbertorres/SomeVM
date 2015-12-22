#ifndef DBR_SL_VARIABLE_HPP
#define DBR_SL_VARIABLE_HPP

namespace dbr
{
	namespace sl
	{
		class Variable
		{
			public:
				enum class Type
				{
					Nil,
					Bool,
					Number,
					String,
					Function,
					Object,
				};

				Variable(Type t);

				Type type();

			private:
				Type typeVal;
		};
	}
}

#endif