#include "Array.hpp"

namespace dbr
{
	namespace svm
	{
		Array::Array(std::size_t len)
		:	lengthVal(len),
			data(nullptr)
		{}

		Array::Array(const Array& other)
		{

		}

		Array::Array(Array&& other)
		{

		}

		Array::~Array()
		{}

		Array& Array::operator=(const Array& other)
		{
			return *this;
		}

		Array& Array::operator=(Array&& other)
		{
			return *this;
		}

		std::size_t Array::length() const
		{
			return lengthVal;
		}

		void Array::clean()
		{
			if(data)
			{
				delete data;
				data = nullptr;
			}
		}
	}
}