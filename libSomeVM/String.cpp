#include "String.hpp"

#include <cstring>

namespace svm
{
    String::String(const char* buf)
        : GC::Handle(nullptr),
        len{ std::strlen(buf) }
    {
        ptr = std::malloc(len);
        std::memcpy(ptr, buf, len);
    }

    String::String(const char* buf, size_t len)
        : GC::Handle(nullptr),
        len{ len }
    {
        ptr = std::malloc(len);
        std::memcpy(ptr, buf, len);
    }

    String::String(GC& gc, const char* buf)
        : GC::Handle(gc, std::strlen(buf)),
        len{ std::strlen(buf) }
    {
        std::memcpy(ptr, buf, len);
    }

    String::String(GC& gc, const char* buf, size_t len)
        : GC::Handle(gc, len),
        len{ len }
    {
        std::memcpy(ptr, buf, len);
    }

    const char* String::data() const
    {
        return static_cast<char*>(ptr);
    }

    size_t String::length() const
    {
        return len;
    }
}
