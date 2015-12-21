#ifndef DBR_SVM_VM_VALUE_HPP
#define DBR_SVM_VM_VALUE_HPP

#include <utility>
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
		using number = float;
		using string = std::string;

		class Value
		{
			public:
				enum class Type
				{
					None,
					Bool,
					Number,
					String,
				};

				Value();
				Value(bool b);
				Value(number f);
				Value(const string& str);

				Value(const Value& other);
				Value(Value&& other);

				Value& operator=(const Value& other);
				Value& operator=(Value&& other);

				~Value();

				std::size_t sizeOf() const;

				void set(nil);
				void set(bool b);
				void set(number f);
				void set(const string& str);

				operator nil() const;
				operator bool() const;
				operator number() const;
				operator string() const;

				Type getType() const;

			private:
				void clean();
				
				void* value;
				Type type;
		};
	}
}

#endif
