#include "Value.hpp"

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
	
	unsigned int BaseValue::getSizeOf() const
	{
		switch(type)
		{
			case ValueType::None:
				return 1;
			case ValueType::Bool:
				return sizeof(bool);
			case ValueType::Number:
				return sizeof(float);
			default:
				return 0;
		}
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
	Value<float>::Value(const float& n)
	{
		value = n;
		setValueType(ValueType::Number);
	}
}
