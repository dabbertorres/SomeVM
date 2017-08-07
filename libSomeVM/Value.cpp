#include "Value.hpp"

#ifdef DEBUG

#include <sstream>

namespace
{
	using Type = svm::Type;

	static const char* typeMap[] =
	{
		"Nil",
		"Bool",
		"Int",
		"Float",
		"Array",
	};

	static const char* asString(Type type)
	{
		return typeMap[static_cast<std::uint8_t>(type)];
	}
}
#endif

namespace svm
{
	Value::Value()
		: value(0)
#ifdef DEBUG			
		, typeVal(Type::Nil)
#endif
	{}

	Value::Value(Nil)
		: Value()
	{}

	Value::Value(Bool b)
#ifdef DEBUG
		: typeVal(Type::Bool)
#endif
	{
		value = b;
	}

	Value::Value(Float f)
#ifdef DEBUG
		: typeVal(Type::Float)
#endif
	{
		reinterpret_cast<Float&>(value) = f;
	}

	Value::Value(const Value& other)
		: value(other.value)
#ifdef DEBUG
		, typeVal(other.typeVal)
#endif
	{}

	Value::Value(Value&& other)
		: value(other.value)
#ifdef DEBUG
		, typeVal(other.typeVal)
#endif
	{
		other.value = 0;

#ifdef DEBUG
		other.typeVal = Type::Nil;
#endif
	}

	Value& Value::operator=(const Value& other)
	{
		value = other.value;

#ifdef DEBUG
		typeVal = other.typeVal;
#endif
		return *this;
	}

	Value& Value::operator=(Value&& other)
	{
		value = other.value;
		other.value = 0;

#ifdef DEBUG
		typeVal = other.typeVal;
		other.typeVal = Type::Nil;
#endif

		return *this;
	}

	Value::~Value()
	{
		cleanup();
	}

	Value& Value::operator=(Nil)
	{
		cleanup();

		value = 0;
#ifdef DEBUG
		typeVal = Type::Nil;
#endif
		return *this;
	}

	Value& Value::operator=(Bool b)
	{
		cleanup();

		reinterpret_cast<Bool&>(value) = b;
#ifdef DEBUG
		typeVal = Type::Bool;
#endif

		return *this;
	}

	Value& Value::operator=(Float f)
	{
		cleanup();

		reinterpret_cast<Float&>(value) = f;
#ifdef DEBUG
		typeVal = Type::Float;
#endif
		return *this;
	}

	Value::operator Nil() const
	{
#ifdef DEBUG
		if (typeVal != Type::Nil)
			throw errorBuilder(Type::Nil, typeVal);
#endif
		return nullptr;
	}

	Value::operator Bool() const
	{
#ifdef DEBUG
		if (typeVal != Type::Bool)
			throw errorBuilder(Type::Bool, typeVal);
#endif
		return reinterpret_cast<const Bool&>(value);
	}

	Value::operator Float() const
	{
#ifdef DEBUG
		if (typeVal != Type::Float)
			throw errorBuilder(Type::Float, typeVal);
#endif
		return reinterpret_cast<const Float&>(value);
	}

	Value::operator Bytes() const
	{
#ifdef DEBUG
		switch (typeVal)
		{
		case Type::Nil:
			return{ 0 };

		case Type::Bool:
			return{ static_cast<std::uint8_t>(reinterpret_cast<const Bool&>(value)) };

		case Type::Float:
		{
			Bytes ret(sizeof(Float));
			std::memcpy(ret.data(), &value, sizeof(Float));

			return ret;
		}

		case Type::Array:
		{
			Array<char>* arr = reinterpret_cast<Array<char>*>(value);

			Bytes ret(arr->length());
			std::memcpy(ret.data(), arr->data(), arr->length());

			return ret;
		}

		default:
			return{ 0 };
		}
#else
        return {0};
#endif
	}

#ifdef DEBUG
	Type Value::type() const
	{
		return typeVal;
	}
#endif

	bool Value::isPointer(std::uint64_t value)
	{
		constexpr auto PTR_MASK = 0xfff0000000000000u;
		return (value & PTR_MASK) == PTR_MASK;
	}

	void Value::freeArray(std::uint64_t value)
	{
		std::free(reinterpret_cast<void*>(value));
	}

	void Value::cleanup()
	{
		if (isPointer(value))
			freeArray(value);
	}

#ifdef DEBUG
	std::runtime_error Value::errorBuilder(Type asked, Type is)
	{
		std::ostringstream oss;
		oss << "Asked for type \"";
		oss << asString(asked);
		oss << "\", when type is \"";
		oss << asString(is);
		oss << '"';

		return std::runtime_error(oss.str());
	}
#endif
}
