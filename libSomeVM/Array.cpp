#include "Array.hpp"

#include <algorithm>

namespace svm
{
    Array::Array()
        : GC::Handle(nullptr),
        offset{ 0 },
        len{ 0 }
    {}

    Array Array::slice(size_t from, size_t to) const
    {
        Array s = *this;
        s.offset = from;
        s.len = to - from;
        return s;
    }

    void Array::realloc(GC& gc)
    {
        ptr = gc.realloc(ptr, static_cast<size_t>(capacity() * growthFactor));
    }

    void* Array::data() const
    {
        return static_cast<char*>(ptr) + offset;
    }

    size_t Array::length() const
    {
        return len;
    }

    void Array::popFront()
    {
        if (len == 0)
            throw std::runtime_error{ "array is empty" };

        ++offset;
        --len;
    }

    void Array::popBack()
    {
        if (len == 0)
            throw std::runtime_error{ "array is empty" };

        --len;
    }
}
