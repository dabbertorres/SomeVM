#include "Value.hpp"

namespace
{
	using Type = dbr::svm::Value::Type;

	static constexpr const char* typeMap[] =
	{
		"Nil",
		"Bool",
		"Number",
		"String",
	};

	static constexpr const char* asString(Type type)
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

		Value::Value(number f)
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

				case Type::Number:
					value = new number(*static_cast<number*>(other.value));
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

			switch(other.typeVal)
			{
				case Type::Nil:
					break;

				case Type::Bool:
					value = new bool(*static_cast<bool*>(other.value));
					break;

				case Type::Number:
					value = new number(*static_cast<number*>(other.value));
					break;

				case Type::String:
					value = new string(*static_cast<string*>(other.value));
					break;
			}

			typeVal = other.typeVal;

			other.clean();

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

				case Type::Number:
					return sizeof(number);

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

		void Value::set(number f)
		{
			if(typeVal != Type::Number)
			{
				clean();

				value = new number(f);

				typeVal = Type::Number;
			}
			else
			{
				*static_cast<number*>(value) = f;
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

		Value::operator number() const
		{
#ifdef DEBUG
			if(typeVal != Type::Number)
				throw errorBuilder(Type::Number, typeVal);
#endif
			if(value)
				return *static_cast<number*>(value);
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
