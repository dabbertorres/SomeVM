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
		return typeMap[static_cast<dbr::svm::byte>(type)];
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
//			return static_cast<byte>(type) & static_cast<byte>(Type::Array);
			return false;
		}

		Value::Value()
			: value(nullptr),
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

		Value::Value(const String& str)
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
			: Value()
		{
			*this = other;
		}

		Value& Value::operator=(const Value& other)
		{
			clean();

			switch(other.typeVal)
			{
				case Type::Nil:
					break;

				case Type::Bool:
					value = new Bool(*static_cast<Bool*>(other.value));
					break;

				case Type::Int:
					value = new Int(*static_cast<Int*>(other.value));

				case Type::Float:
					value = new Float(*static_cast<Float*>(other.value));
					break;

				case Type::String:
					value = new String(*static_cast<String*>(other.value));
					break;
			}

			typeVal = other.typeVal;

			return *this;
		}

		Value& Value::operator=(Value&& other)
		{
			clean();

			value = other.value;

			typeVal = other.typeVal;

			return *this;
		}

		Value::~Value()
		{
			clean();
		}

		std::size_t Value::sizeOf() const
		{
			switch(typeVal)
			{
				case Type::Nil:
					return 1;

				case Type::Bool:
					return sizeof(Bool);

				case Type::Int:
					return sizeof(Int);

				case Type::Float:
					return sizeof(Float);

				case Type::String:
					return value ? static_cast<String*>(value)->size() : 0;
			}

			return 0;
		}

		void Value::set(Nil)
		{
			clean();

			typeVal = Type::Nil;
		}

		void Value::set(Bool b)
		{
			if(typeVal != Type::Bool)
			{
				clean();

				value = new Bool(b);

				typeVal = Type::Bool;
			}
			else
			{
				*static_cast<Bool*>(value) = b;
			}
		}

		void Value::set(Int i)
		{
			if(typeVal != Type::Int)
			{
				clean();

				value = new Int(i);

				typeVal = Type::Int;
			}
			else
			{
				*static_cast<Int*>(value) = i;
			}
		}

		void Value::set(Float f)
		{
			if(typeVal != Type::Float)
			{
				clean();

				value = new Float(f);

				typeVal = Type::Float;
			}
			else
			{
				*static_cast<Float*>(value) = f;
			}
		}

		void Value::set(const String& str)
		{
			if(typeVal != Type::String)
			{
				clean();

				value = new String(str);

				typeVal = Type::String;
			}
			else
			{
				*static_cast<String*>(value) = str;
			}
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
			if(value)
				return *static_cast<Bool*>(value);
			else
				return false;
		}

		Value::operator Int() const
		{
#ifdef DEBUG
			if(typeVal != Type::Int)
				throw errorBuilder(Type::Int, typeVal);
#endif
			if(value)
				return *static_cast<Int*>(value);
			else
				return 0;
		}

		Value::operator Float() const
		{
#ifdef DEBUG
			if(typeVal != Type::Float)
				throw errorBuilder(Type::Float, typeVal);
#endif
			if(value)
				return *static_cast<Float*>(value);
			else
				return 0;
		}

		Value::operator String() const
		{
#ifdef DEBUG
			if(typeVal != Type::String)
				throw errorBuilder(Type::String, typeVal);
#endif
			if(value)
				return *static_cast<String*>(value);
			else
				return "";
		}

		Value::operator Bytes() const
		{
			Bytes ret(sizeOf());

			if(value)
			{
				auto byteBeg = static_cast<byte*>(value);

				std::copy(byteBeg, byteBeg + ret.size(), ret.begin());
			}
			else
			{
				std::fill(ret.begin(), ret.end(), 0);
			}

			return ret;
		}

		Value::Type Value::type() const
		{
			return typeVal;
		}

		bool Value::isArray() const
		{
			return isArray(typeVal);
		}

		void Value::clean()
		{
			if(value)
			{
				delete value;
				value = nullptr;
			}
		}
	}
}
