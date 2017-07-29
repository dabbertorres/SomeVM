#pragma once

#include "libSomeVM.hpp"

#include <cstdint>
#include <stdexcept>

#include "Array.hpp"

namespace svm
{
	using Bytes = Array<std::uint8_t>;

	using Nil = std::nullptr_t;
	using Bool = bool;
	using Float = double;

	static_assert(std::numeric_limits<Float>::is_iec559, "IEEE 754 floating point standard is expected.");
	static_assert(sizeof(double) * CHAR_BIT == 64, "Unexpected size for double-precision float. Should be 64 bits");

	// why? well assuming the above allows us to do some fun stuff to make things fast
	//
	// IEEE 754 double looks like this in memory:
	// SExponent---Mantissa--------------------------------------------
	// where:
	// S = Sign				(1 bit)
	// Exponent = Exponent	(11 bits)
	// Mantissa = Mantissa	(52 bits)
	// - = continuation of previous word
	//
	// NaN values are represented as:
	// -11111111111----------------------------------------------------
	// (All Exponent bits are set, the rest don't matter)
	// 
	// Notice the Sign bit doesn't matter
	// we can use this to tag pointers (1) vs values (0)
	//
	// "But wait! That means pointers don't fully fit in the value they should!"
	// Actually they will! 64-bit machines only use 48-bits for pointers!

#ifdef DEBUG
	enum class SVM_API Type : std::uint8_t
	{
		Nil = 0,
		Bool = 1,
		Float = 2,
		Array = 3,
	};
#endif

	class SVM_API Value
	{
	public:
		Value();
		Value(Nil);
		Value(Bool b);
		Value(Float f);

		template<typename T>
		Value(Array<T> arr);

		Value(const Value& other);
		Value(Value&& other);

		Value& operator=(const Value& other);
		Value& operator=(Value&& other);

		~Value();

		Value& operator=(Nil);
		Value& operator=(Bool b);
		Value& operator=(Float f);

		template<typename T>
		Value& operator=(Array<T> arr);

		operator Nil() const;
		operator Bool() const;
		operator Float() const;

		template<typename T>
		operator Array<T>() const;

		operator Bytes() const;

#ifdef DEBUG
		Type type() const;
#endif

	private:
		static bool isPointer(std::uint64_t value);

		template<typename T>
		static std::uint64_t newArray(Array<T> arr);
		static void freeArray(std::uint64_t value);

		void cleanup();

		std::uint64_t value;

#ifdef DEBUG
		static std::runtime_error errorBuilder(Type asked, Type is);
		Type typeVal;
#endif
	};

	template<typename T>
	Value::Value(Array<T> arr)
#ifdef DEBUG
		: typeVal(Type::Array)
#endif
	{
		value = newArray(arr);
	}

	template<typename T>
	Value& Value::operator=(Array<T> arr)
	{
		if (isPointer(value))
			freeArray(value);

		value = newArray(arr);

#ifdef DEBUG
		typeVal = Type::Array;
#endif

		return *this;
	}

	template<typename T>
	Value::operator Array<T>() const
	{
#ifdef DEBUG
		if (typeVal != Type::Array)
			throw errorBuilder(Type::Array, typeVal);
#endif
		return *reinterpret_cast<Array<T>*>(value);
	}

	template<typename T>
	std::uint64_t Value::newArray(Array<T> arr)
	{
		std::uint64_t ret = reinterpret_cast<std::uint64_t>(std::malloc(sizeof(Array<T>)));
		new (reinterpret_cast<void*>(ret)) Array<T>(arr);
		return ret;
	}
}
