#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>

using byte = unsigned char;
using nil = std::nullptr_t;

namespace lng
{
	enum class ValueType : byte
	{
	    None,
	    Bool,
	    Number,
	};
	
	union None
	{
		char bytes[1];
		std::nullptr_t value;
	};
	
	union Bool
	{
		char bytes[1];
		bool value;
	};
	
	union Float
	{
		char bytes[sizeof(float)];
		float value;
	};

	class BaseValue
	{
		public:
			BaseValue();

			void setValueType(ValueType t);
			ValueType getValueType() const;
			unsigned int getSizeOf() const;

		private:
			ValueType type;
	};

	template<typename T>
	class Value : public BaseValue
	{
		public:
			Value();
			Value(const T& v);

			T getValue() const;

		private:
			T value;
	};
	
	template<typename T>
	Value<T>::Value()
	{
	}

	template<typename T>
	T Value<T>::getValue() const
	{
		return value;
	}
}

#endif // VALUE_HPP
