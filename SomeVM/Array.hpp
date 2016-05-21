#ifndef DBR_SVM_ARRAY_HPP
#define DBR_SVM_ARRAY_HPP

#include <cstdlib>

namespace dbr
{
	namespace svm
	{
		class Array
		{
		public:
			Array(std::size_t len);

			Array(const Array& other);
			Array(Array&& other);

			~Array();

			Array& operator=(const Array& other);
			Array& operator=(Array&& other);

			std::size_t length() const;

		private:
			void clean();

			std::size_t lengthVal;
			void* data;
		};
	}
}

#endif
