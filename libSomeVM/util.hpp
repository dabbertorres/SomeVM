#pragma once

#include <cstdint>

namespace svm
{
    inline uint8_t operator"" _u8(unsigned long long int v)
    {
        return static_cast<uint8_t>(v);
    }

    inline uint16_t operator"" _u16(unsigned long long int v)
    {
        return static_cast<uint16_t>(v);
    }

    inline uint32_t operator"" _u32(unsigned long long int v)
    {
        return static_cast<uint32_t>(v);
    }

    inline uint64_t operator"" _u64(unsigned long long int v)
    {
        return static_cast<uint64_t>(v);
    }

    inline int8_t operator"" _i8(unsigned long long int v)
    {
        return static_cast<int8_t>(v);
    }

    inline int16_t operator"" _i16(unsigned long long int v)
    {
        return static_cast<int16_t>(v);
    }

    inline int32_t operator"" _i32(unsigned long long int v)
    {
        return static_cast<int32_t>(v);
    }

    inline int64_t operator"" _i64(unsigned long long int v)
    {
        return static_cast<int64_t>(v);
    }
}
