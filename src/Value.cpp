#include "Value.hpp"

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
		std::string msg = "Asked for type \"";
		msg += asString(asked);
		msg += "\", when type is \"";
		msg += asString(is);
		msg += '"';

		return std::runtime_error{msg};
	}
}

namespace dbr
{
	namespace svm
	{
		Value::Value()
		:	value(nullptr),
			typeVal(Type::Nil)
		{}

		Value::Value(bool b)
		:	Value()
		{
			set(b);
		}

		Value::Value(int i)
		:	Value()
		{
			set(i);
		}

		Value::Value(float f)
		:	Value()
		{
			set(f);
		}

		Value::Value(const string& str)
		:	Value()
		{
			set(str);
		}

		Value::Value(const Value& other)
		:	Value()
		{
			*this = other;
		}

		Value::Value(Value&& other)
		:	Value()
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
					value = new bool(*static_cast<bool*>(other.value));
					break;

				case Type::Int:
					value = new int(*static_cast<int*>(other.value));

				case Type::Float:
					value = new float(*static_cast<float*>(other.value));
					break;

				case Type::String:
					value = new string(*static_cast<string*>(other.value));
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
					return sizeof(bool);

				case Type::Int:
					return sizeof(int);

				case Type::Float:
					return sizeof(float);

				case Type::String:
					return value ? static_cast<string*>(value)->size() : 0;
			}

			return 0;
		}

		void Value::set(nil)
		{
			clean();

			typeVal = Type::Nil;
		}

		void Value::set(bool b)
		{
			if(typeVal != Type::Bool)
			{
				clean();

				value = new bool(b);

				typeVal = Type::Bool;
			}
			else
			{
				*static_cast<bool*>(value) = b;
			}
		}

		void Value::set(int i)
		{
			if(typeVal != Type::Int)
			{
				clean();

				value = new int(i);

				typeVal = Type::Int;
			}
			else
			{
				*static_cast<int*>(value) = i;
			}
		}

		void Value::set(float f)
		{
			if(typeVal != Type::Float)
			{
				clean();

				value = new int(f);

				typeVal = Type::Float;
			}
			else
			{
				*static_cast<int*>(value) = f;
			}
		}

		void Value::set(const string& str)
		{
			if(typeVal != Type::String)
			{
				clean();

				value = new string(str);

				typeVal = Type::String;
			}
			else
			{
				*static_cast<string*>(value) = str;
			}
		}

		Value::operator nil() const
		{
#ifdef DEBUG
			if(typeVal != Type::Nil)
				throw errorBuilder(Type::Nil, typeVal);
#endif
			return nullptr;
		}

		Value::operator bool() const
		{
#ifdef DEBUG
			if(typeVal != Type::Bool)
				throw errorBuilder(Type::Bool, typeVal);
#endif
			if(value)
				return *static_cast<bool*>(value);
			else
				return false;
		}

		Value::operator int() const
		{
#ifdef DEBUG
			if(typeVal != Type::Int)
				throw errorBuilder(Type::Int, typeVal);
#endif
			if(value)
				return *static_cast<int*>(value);
			else
				return 0;
		}

		Value::operator float() const
		{
#ifdef DEBUG
			if(typeVal != Type::Float)
				throw errorBuilder(Type::Float, typeVal);
#endif
			if(value)
				return *static_cast<float*>(value);
			else
				return 0;
		}

		Value::operator string() const
		{
#ifdef DEBUG
			if(typeVal != Type::String)
				throw errorBuilder(Type::String, typeVal);
#endif
			if(value)
				return *static_cast<string*>(value);
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
