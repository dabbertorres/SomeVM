#pragma once

#include "GC.hpp"

namespace svm
{
    // immutable string
    class String : public GC::Handle
    {
    public:
        // allocate without the GC (ie: constants at build-time)
        String(const char* buf);
        String(const char* buf, size_t len);

        // allocate with the GC (at runtime)
        String(GC& gc, const char* buf);
        String(GC& gc, const char* buf, size_t len);

        const char* data() const;
        size_t length() const;

    private:
        size_t len;
    };
}
