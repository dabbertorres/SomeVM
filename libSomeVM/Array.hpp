#pragma once

#include <memory>
#include <cstring>

namespace svm
{
	template<typename T>
	class Array
	{
	public:
		Array();
		Array(std::uint64_t len);
		Array(const T* data, std::uint64_t len);

		Array(const Array& other);
		Array(Array&& other);

		~Array() = default;

		Array& operator=(const Array& other);
		Array& operator=(Array&& other);

		T& operator[](std::uint64_t idx);
		const T& operator[](std::uint64_t idx) const;

		T* data();
		const T* data() const;

		std::uint64_t length() const;

	private:
		std::uint64_t lengthVal;
		std::unique_ptr<T[]> ptr;
	};

	template<typename T>
	Array<T>::Array()
		: lengthVal{ 0 },
		ptr{ nullptr }
	{}

	template<typename T>
	Array<T>::Array(std::uint64_t len)
		: lengthVal{ len },
		ptr{ std::make_unique<T[]>(len) }
	{}

	template<typename T>
	Array<T>::Array(const T* data, std::uint64_t len)
		: lengthVal{ len },
		ptr{ std::make_unique<T[]>(len) }
	{
		std::memcpy(ptr.get(), data, lengthVal * sizeof(T));
	}

	template<typename T>
	Array<T>::Array(const Array& other)
		: lengthVal{ other.lengthVal },
		ptr{ std::make_unique<T[]>(other.lengthVal) }
	{
		std::memcpy(ptr.get(), other.ptr.get(), lengthVal * sizeof(T));
	}

	template<typename T>
	Array<T>::Array(Array&& other)
		: lengthVal(other.lengthVal),
		ptr{ std::move(other.ptr) }
	{
		other.lengthVal = 0;
	}

	template<typename T>
	Array<T>& Array<T>::operator=(const Array& other)
	{
		lengthVal = other.lengthVal;
		ptr = std::make_unique<T[]>(lengthVal);
		std::memcpy(ptr.get(), other.ptr.get(), lengthVal * sizeof(T));
		return *this;
	}

	template<typename T>
	Array<T>& Array<T>::operator=(Array&& other)
	{
		lengthVal = other.lengthVal;
		ptr = std::move(other.ptr);
		return *this;
	}

	template<typename T>
	T& Array<T>::operator[](std::uint64_t idx)
	{
		if (idx < lengthVal)
			return ptr[idx];
		else
			throw std::out_of_range{ "Index greater-than or equal-to length of array" };

	}

	template<typename T>
	const T& Array<T>::operator[](std::uint64_t idx) const
	{
		if (idx < lengthVal)
			return ptr[idx];
		else
			throw std::out_of_range{};
	}

	template<typename T>
	T* Array<T>::data()
	{
		return ptr.get();
	}

	template<typename T>
	const T* Array<T>::data() const
	{
		return ptr.get();
	}

	template<typename T>
	std::uint64_t Array<T>::length() const
	{
		return lengthVal;
	}
}
