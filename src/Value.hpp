#ifndef DBR_SVM_VALUE_HPP
#define DBR_SVM_VALUE_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace dbr
{
	namespace svm
	{
		using byte = std::uint8_t;
		using Bytes = std::vector<byte>;

		using nil = std::nullptr_t;
		using string = std::string;

		class Value
		{
			public:
				enum class Type : byte
				{
					Nil,
					Bool,
					Int,
					Float,
					String,
				};

				Value();
				Value(bool b);
				Value(int i);
				Value(float f);
				Value(const string& str);

				Value(const Value& other);
				Value(Value&& other);

				Value& operator=(const Value& other);
				Value& operator=(Value&& other);

				~Value();

				std::size_t sizeOf() const;

				void set(nil);
				void set(bool b);
				void set(int i);
				void set(float f);
				void set(const string& str);

				operator nil() const;
				operator bool() const;
				operator int() const;
				operator float() const;
				operator string() const;
				operator Bytes() const;

				Type type() const;

			private:
				void clean();
				
				void* value;
				Type typeVal;
		};

		using Constants = std::vector<Value>;
	}
}

#endif
