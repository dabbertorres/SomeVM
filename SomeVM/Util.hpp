#ifndef DBR_SVM_UTIL_HPP
#define DBR_SVM_UTIL_HPP

#include <string>
#include <type_traits>

namespace dbr
{
	namespace svm
	{
		namespace util
		{
			constexpr std::size_t strlen(const char* str)
			{
				return str[0] != 0 ? strlen(str + 1) + 1 : 0;
			}

			void toLower(std::string& str);

			std::string stringTrim(const std::string& str);

			bool isInt(const std::string& str);
			bool isFloat(const std::string& str);
			bool isBool(const std::string& str);
			bool isHex(const std::string& str);

			bool strToBool(const std::string& str);
		}
	}
}

#endif
