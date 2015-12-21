#include "Value.hpp"

namespace
{
	struct Pair
	{
		dbr::svm::Value::Type type;
		const char* name;
	};

	static constexpr Pair typeMap[] =
	{
		{dbr::svm::Value::Type::None, "None"},
		{dbr::svm::Value::Type::Bool, "Bool"},
		{dbr::svm::Value::Type::Number, "Number"},
		{dbr::svm::Value::Type::String, "String"},
	};

	static constexpr const char* typeToStringHelper(dbr::svm::Value::Type type, std::size_t idx)
	{
		return type == typeMap[idx].type ? typeMap[idx].name : typeToStringHelper(type, idx + 1);
	}

	static constexpr const char* typeToString(dbr::svm::Value::Type type)
	{
		return typeToStringHelper(type, 0);
	}

	static std::runtime_error errorBuilder(dbr::svm::Value::Type asked, dbr::svm::Value::Type is)
	{
		std::string msg = "Asked for type \"";
		msg += typeToString(asked);
		msg += "\", when type is \"";
		msg += typeToString(is);
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
			type(Type::None)
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

			switch(other.type)
			{
				case Type::None:
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

			type = other.type;

			return *this;
		}

		Value& Value::operator=(Value&& other)
		{
			clean();

			switch(other.type)
			{
				case Type::None:
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

			type = other.type;

			other.clean();

			return *this;
		}

		Value::~Value()
		{
			clean();
		}

		std::size_t Value::sizeOf() const
		{
			switch(type)
			{
				case Type::None:
					return 1;

				case Type::Bool:
					return sizeof(bool);

				case Type::Number:
					return sizeof(number);

				case Type::String:
					return sizeof(string) + (value ? static_cast<string*>(value)->size() : 0);
			}

			return 0;
		}

		void Value::set(nil)
		{
			clean();

			type = Type::None;
		}

		void Value::set(bool b)
		{
			if(type != Type::Bool)
			{
				clean();

				value = new bool(b);

				type = Type::Bool;
			}
			else
			{
				*static_cast<bool*>(value) = b;
			}
		}

		void Value::set(number f)
		{
			if(type != Type::Number)
			{
				clean();

				value = new number(f);

				type = Type::Number;
			}
			else
			{
				*static_cast<number*>(value) = f;
			}
		}

		void Value::set(const string& str)
		{
			if(type != Type::String)
			{
				clean();

				value = new string(str);

				type = Type::String;
			}
			else
			{
				*static_cast<string*>(value) = str;
			}
		}

		Value::operator nil() const
		{
#ifdef DEBUG
			if(type != Type::None)
				throw errorBuilder(Type::None, type);
#endif
			return nullptr;
		}

		Value::operator bool() const
		{
#ifdef DEBUG
			if(type != Type::Bool)
				throw errorBuilder(Type::Bool, type);
#endif
			if(value)
				return *static_cast<bool*>(value);
			else
				return false;
		}

		Value::operator number() const
		{
#ifdef DEBUG
			if(type != Type::Number)
				throw errorBuilder(Type::Number, type);
#endif
			if(value)
				return *static_cast<number*>(value);
			else
				return 0;
		}

		Value::operator string() const
		{
#ifdef DEBUG
			if(type != Type::String)
				throw errorBuilder(Type::String, type);
#endif
			if(value)
				return *static_cast<string*>(value);
			else
				return "";
		}

		Value::Type Value::getType() const
		{
			return type;
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
