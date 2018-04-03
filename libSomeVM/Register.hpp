#pragma once

#include <cstdint>

namespace svm
{
    using Register = uint16_t;

    // user defined literal
    inline Register operator "" _r(unsigned long long int v)
    {
        return static_cast<Register>(v);
    }

    // if reg is referring to a constant, returns true, and adjusts reg for use as a valid index
    // otherwise, returns false, and does nothing to reg
    inline bool isConstant(Register& reg)
    {
        constexpr uint16_t constantSentinel = 0x8000;
        bool isC = (reg & constantSentinel) != 0;
        reg &= ~constantSentinel;
        return isC;
    }
}
