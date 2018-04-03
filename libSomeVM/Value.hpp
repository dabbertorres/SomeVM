#pragma once

#include <cstdint>
#include <variant>

#include "Array.hpp"
#include "GC.hpp"
#include "String.hpp"

namespace svm
{
    using Bool = bool;
    using Int = int64_t;
    using Float = double;

    class Object : public GC::Handle
    {
    public:
        Object(GC& gc, size_t size)
            : GC::Handle(gc, size)
        {}
    };

    using Value = std::variant<Bool, Int, Float, String, Array, Object>;

    template<typename T>
    inline constexpr size_t typeIndex()
    {
        if constexpr (std::is_same_v<T, Bool>)
            return 0;
        else if constexpr (std::is_same_v<T, Int>)
            return 1;
        else if constexpr (std::is_same_v<T, Float>)
            return 2;
        else if constexpr (std::is_same_v<T, String>)
            return 3;
        else if constexpr (std::is_same_v<T, Array>)
            return 4;
        else if constexpr (std::is_same_v<T, Object>)
            return 5;
        else
            static_assert(std::false_type, "type T not held in Value");
    }
}
