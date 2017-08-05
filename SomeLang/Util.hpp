#pragma once

#include <string>

namespace sl
{
    namespace util
    {
        namespace priv
        {
            template<typename T, typename Tn, typename... Ts>
            constexpr std::uint64_t maxSize(std::uint64_t max)
            {
                return maxSize<Tn, Ts...>(maxSize<T>(max));
            }

            template<typename T>
            constexpr std::uint64_t maxSize(std::uint64_t max)
            {
                return max >= sizeof(T) ? max : sizeof(T);
            }
        }

        template<typename... Ts>
        constexpr std::uint64_t maxSize()
        {
            return priv::maxSize<Ts...>(0);
        }

        void toLower(std::string& str);
        void toUpper(std::string& str);

        std::string stringTrim(const std::string& str);

        bool isInt(const std::string& str);
        bool isNum(const std::string& str);
        bool isBool(const std::string& str);
        bool isHex(const std::string& str);

        bool strToBool(const std::string& str);
    }
}
