#include "Value.hpp"

#include <sstream>

namespace
{
	using Type = dbr::svm::Value::Type;

	static const char* typeMap[] =
	{
		"Nil",
		"Bool",
		"Int",
		"Float",
		"String",
	};

	static const char* asString(Type type)
	{
		return typeMap[static_cast<std::uint8_t>(type)];
	}

	static std::runtime_error errorBuilder(Type asked, Type is)
	{
		std::ostringstream oss;
		oss << "Asked for type \"";
		oss << asString(asked);
		oss << "\", when type is \"";
		oss << asString(is);
		oss << '"';

		return std::runtime_error(oss.str());
	}
}

namespace dbr
{
	namespace svm
	{
		bool Value::isArray(Type type)
		{
//			return static_cast<std::uint8_t>(type) & static_cast<std::uint8_t>(Type::Array);
			return false;
		}

		Value::Value()
			: value(0),
			typeVal(Type::Nil)
		{}

		Value::Value(Bool b)
			: Value()
		{
			set(b);
		}

		Value::Value(Int i)
			: Value()
		{
			set(i);
		}

		Value::Value(Float f)
			: Value()
		{
			set(f);
		}

		Value::Value(String str)
			: Value()
		{
			set(str);
		}

		Value::Value(const Value& other)
			: Value()
		{
			*this = other;
		}

		Value::Value(Value&& other)
			: value(other.value),
			typeVal(other.typeVal)
		{
			*this = std::move(other);
		}

		Value& Value::operator=(const Value& other)
		{
			switch(other.typeVal)
			{
				case Type::Nil:
					set(nullptr);
					break;

				case Type::Bool:
					set(reinterpret_cast<const Bool&>(other.value));
					break;

				case Type::Int:
					set(reinterpret_cast<Int>(other.value));
					break;

				case Type::Float:
					set(*reinterpret_cast<const Float*>(&other.value));
					break;

				case Type::String:
					set(*static_cast<String*>(other.value));
					break;
			}
			
			return *this;
		}

		Value& Value::operator=(Value&& other)
		{
			value = other.value;
			typeVal = other.typeVal;

			other.value = 0;
			other.typeVal = Type::Nil;

			return *this;
		}

		Value::~Value()
		{
			if(value)
			{
				checkString();
				value = 0;
			}
		}

		void Value::set(Nil)
		{
			checkString();
			typeVal = Type::Nil;
		}

		void Value::set(Bool b)
		{
			checkString();
			reinterpret_cast<Bool&>(value) = b;
			typeVal = Type::Bool;
		}

		void Value::set(Int i)
		{
			checkString();
			reinterpret_cast<Int&>(value) = i;
			typeVal = Type::Int;
		}

		void Value::set(Float f)
		{
			checkString();
			reinterpret_cast<Float&>(value) = f;
			typeVal = Type::Float;
		}

		void Value::set(String str)
		{
			checkString();
			// store a pointer to a String
			value = new String(str);
			typeVal = Type::String;
		}

		Value::operator Nil() const
		{
#ifdef DEBUG
			if(typeVal != Type::Nil)
				throw errorBuilder(Type::Nil, typeVal);
#endif
			return nullptr;
		}

		Value::operator Bool() const
		{
#ifdef DEBUG
			if(typeVal != Type::Bool)
				throw errorBuilder(Type::Bool, typeVal);
#endif
			return reinterpret_cast<const Bool&>(value);
		}

		Value::operator Int() const
		{
#ifdef DEBUG
			if(typeVal != Type::Int)
				throw errorBuilder(Type::Int, typeVal);
#endif
			return reinterpret_cast<Int>(value);
		}

		Value::operator Float() const
		{
#ifdef DEBUG
			if(typeVal != Type::Float)
				throw errorBuilder(Type::Float, typeVal);
#endif
			return *reinterpret_cast<const Float*>(&value);
		}

		Value::operator String() const
		{
#ifdef DEBUG
			if(typeVal != Type::String)
				throw errorBuilder(Type::String, typeVal);
#endif
			return *static_cast<String*>(value);
		}

		Value::operator Bytes() const
		{
			switch(typeVal)
			{
				case Type::Nil:
					return{};

				case Type::Bool:
					return{static_cast<std::uint8_t>(reinterpret_cast<const Bool&>(value))};

				case Type::Int:
				case Type::Float:
				{
					Bytes ret(MEM_SIZE);

					std::memcpy(ret.data(), &value, MEM_SIZE);

					return ret;
				}

				case Type::String:
				{
					String* str = static_cast<String*>(value);

					Bytes ret(str->size());

					std::memcpy(ret.data(), str->data(), str->size());

					return ret;
				}

				default:
					return{};
			}
		}

		Value::Type Value::type() const
		{
			return typeVal;
		}

		bool Value::isArray() const
		{
			return isArray(typeVal);
		}

		void Value::checkString()
		{
			if(typeVal == Type::String)
				delete static_cast<String*>(value);
		}
	}
}
