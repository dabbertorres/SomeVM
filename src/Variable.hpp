#ifndef LNG_VARIABLE_HPP
#define LNG_VARIABLE_HPP

namespace lng
{
	class Variable
	{
		public:
			enum class TokenType
			{
				None,
				Bool,
				Number,
				String,
				Function,
				Object,
			};

			Variable(TokenType);

			TokenType type();

		private:
			TokenType typeVal;
	};
}

#endif	// LNG_VARIABLE_HPP