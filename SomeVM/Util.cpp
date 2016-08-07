#include "Util.hpp"

#include <locale>
#include <algorithm>

namespace dbr
{
	namespace svm
	{
		namespace util
		{
			void toLower(std::string& str)
			{
				std::locale lcl;
				std::transform(str.begin(), str.end(), str.begin(), [&](char c) { return std::tolower(c, lcl); });
			}

			std::string stringTrim(const std::string& str)
			{
				std::locale lcl;

				auto isspace = [&](char c) { return std::isspace(c, lcl); };

				auto left = std::find_if_not(str.begin(), str.end(), isspace);
				auto right = std::find_if_not(str.rbegin(), str.rend(), isspace).base();

				return{left, right};
			}

			bool isInt(const std::string& str)
			{
				std::size_t idx = 0;

				while((idx = str.find_first_not_of("0123456789", idx + 1)) != std::string::npos)
				{
					if(idx == 0 && (str[0] == '+' || str[0] == '-'))
						continue;
					else
						return false;
				}

				return true;
			}

			bool isFloat(const std::string& str)
			{
				std::size_t idx = std::numeric_limits<std::size_t>::max();
				bool hasDecimal = false;

				while((idx = str.find_first_not_of("0123456789", idx + 1)) != std::string::npos)
				{
					if(idx == 0 && (str[0] == '+' || str[0] == '-'))
					{
						continue;
					}
					else if(str[idx] == '.' && !hasDecimal)
					{
						hasDecimal = true;
						continue;
					}
					else
					{
						return false;
					}
				}

				return true;
			}

			bool isBool(const std::string& str)
			{
				return str == "true" || str == "false";
			}

			bool isHex(const std::string& str)
			{
				if(str[0] != '0' || str[1] != 'x')
					return false;

				return str.find_first_not_of("0123456789", 2) != std::string::npos;
			}

			bool strToBool(const std::string& str)
			{
				if(!isBool(str))
					return false;

				return str[0] == 't';
			}
		}
	}
}
