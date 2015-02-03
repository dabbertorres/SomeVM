#include "Value.hpp"
#include <iostream>
namespace lng
{
	BaseValue::BaseValue()
	:	type(ValueType::None)
	{}
	
	void BaseValue::setValueType(ValueType t)
	{
		type = t;
	}
	
	ValueType BaseValue::getValueType() const
	{
		return type;
	}
	
	template<>
	Value<nil>::Value(const nil&)
	{
		value = nullptr;
		setValueType(ValueType::None);
	}
	
	template<>
	Value<bool>::Value(const bool& b)
	{
		value = b;
		setValueType(ValueType::Bool);
	}

	template<>
	Value<double>::Value(const double& n)
	{
		value = n;
		setValueType(ValueType::Number);
	}

	template<>
	Value<Array>::Value(const Array& ar)
	{
		value = ar;
		setValueType(ValueType::Array);
	}

	template<>
	Value<Object>::Value(const Object& o)
	{
		value = o;
		setValueType(ValueType::Object);
	}
	
	template<>
	Value<Function>::Value(const Function& f)
	{
		value = f;
		setValueType(ValueType::Function);
	}
}
