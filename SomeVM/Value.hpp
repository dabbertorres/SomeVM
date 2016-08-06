#ifndef DBR_SVM_VALUE_HPP
#define DBR_SVM_VALUE_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace dbr
{
	namespace svm
	{
		using Bytes = std::vector<std::uint8_t>;

		using Nil = std::nullptr_t;
		using Bool = bool;
		using Int = long long int;
		using Float = double;
		using String = std::string;

		class Value
		{
		public:
			enum class Type : std::uint8_t
			{
				Nil = 0,
				Bool = 1,
				Int = 2,
				Float = 3,
				String = 4,
			};

			static constexpr std::uint64_t MEM_SIZE = sizeof(void*);

			static bool isArray(Type type);

			Value();
			Value(Bool b);
			Value(Int i);
			Value(Float f);
			Value(String str);

			Value(const Value& other);
			Value(Value&& other);

			Value& operator=(const Value& other);
			Value& operator=(Value&& other);

			~Value();

			void set(Nil);
			void set(Bool b);
			void set(Int i);
			void set(Float f);
			void set(String str);

			operator Nil() const;
			operator Bool() const;
			operator Int() const;
			operator Float() const;
			operator String() const;

			operator Bytes() const;

			Type type() const;
			bool isArray() const;

		private:
			void checkString();

			void* value;
			Type typeVal;
		};

		using Constants = std::vector<Value>;
	}
}

#endif
