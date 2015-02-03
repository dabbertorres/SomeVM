#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>

#include "Array.hpp"
#include "Object.hpp"
#include "Function.hpp"

using byte = unsigned char;
using nil = std::nullptr_t;

namespace lng
{
	enum class ValueType : byte
	{
	    None,
	    Bool,
	    Number,
	    Array,
	    Function,
	    Object
	};

	class BaseValue
	{
		public:
			BaseValue();

			void setValueType(ValueType t);
			ValueType getValueType() const;

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
