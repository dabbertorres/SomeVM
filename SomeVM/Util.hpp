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
			namespace priv
			{
				template<typename T, typename T1, typename... Ts>
				constexpr std::size_t maxSize(std::size_t max)
				{
					return maxSize<T1, Ts...>(max >= sizeof(T) ? max : sizeof(T));
				}

				template<typename T>
				constexpr std::size_t maxSize(std::size_t max)
				{
					return max >= sizeof(T) ? max : sizeof(T);
				}

			}

			constexpr std::size_t strlen(const char* str)
			{
				return str[0] != 0 ? strlen(str + 1) + 1 : 0;
			}

			template<typename... Ts>
			constexpr std::size_t maxSize()
			{
				return priv::maxSize<Ts...>(0);
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
