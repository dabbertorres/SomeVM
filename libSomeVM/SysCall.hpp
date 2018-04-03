#pragma once

#include <cstdint>

namespace svm
{
    enum class SysCall : int16_t
    {
        PrintBool,
        PrintInt,
        PrintFloat,
        Print,
    };
}
