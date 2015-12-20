#ifndef LNG_VARIABLE_HPP
#define LNG_VARIABLE_HPP

namespace lng
{
	class Variable
	{
		public:
			enum class Type
			{
				None,
				Bool,
				Number,
				String,
				Function,
				Object,
			};

			Variable(Type);

			Type type();

		private:
			Type typeVal;
	};
}

#endif	// LNG_VARIABLE_HPP