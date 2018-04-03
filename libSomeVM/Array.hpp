#pragma once

#include <cstdint>
#include <cstring>
#include <memory>

#include "GC.hpp"

namespace svm
{
    class Array : public GC::Handle
    {
        static constexpr double growthFactor = 1.5;

        // a default/zero'd Array has a capacity of 0.
        // if it is pushed to, we'll allocate at least this much
        static constexpr size_t minCap = 8;

    public:
        Array();

        template<typename T>
        Array(GC& gc, size_t len);

        Array(const Array& other) = default;
        Array(Array&& other) = default;

        ~Array() = default;

        Array& operator=(const Array& other) = default;
        Array& operator=(Array&& other) = default;

        Array slice(size_t from, size_t to) const;

        void realloc(GC& gc);

        void* data() const;

        size_t length() const;

        template<typename T>
        T& at(size_t idx) const;

        // returns false if the Array needs to be reallocated
        template<typename T>
        bool insert(size_t idx, T t);

        // returns false if the Array needs to be reallocated
        template<typename T>
        bool prepend(T t);

        // returns false if the Array needs to be reallocated
        template<typename T>
        bool append(T t);

        template<typename T>
        void remove(size_t idx);

        void popFront();
        void popBack();

    private:
        size_t offset;
        size_t len;
    };

    template<typename T>
    Array::Array(GC& gc, size_t len)
        : GC::Handle(gc, len * sizeof(T)),
        offset{ 0 },
        len{ len }
    {}

    template<typename T>
    T& Array::at(size_t idx) const
    {
        if (idx < len)
            return *reinterpret_cast<T*>(ptr + (offset + idx) * sizeof(T));
        else
            throw std::out_of_range{ "array index out of bounds" };
    }

    template<typename T>
    bool Array::insert(size_t idx, T t)
    {
        if (idx > len)
            throw std::out_of_range{ "array index out of bounds" };

        // check if we need to realloc
        size_t cap = capacity();
        if (cap == len * sizeof(T))
            return false;

        void* dest = static_cast<char*>(ptr) + (offset + idx) * sizeof(T);
        std::memcpy(dest, static_cast<char*>(dest) + 1 * sizeof(T), (len - idx) * sizeof(T));
        ++len;

        *reinterpret_cast<T*>(dest) = t;
        return true;
    }

    template<typename T>
    bool Array::prepend(T t)
    {
        // we can just move our offset!
        if (offset > 0)
        {
            *reinterpret_cast<T*>(ptr + offset * sizeof(T)) = t;
            ++len;
            --offset;
            return true;
        }
        // gotta copy everything over
        else
        {
            return insert(0, t);
        }
    }

    template<typename T>
    bool Array::append(T t)
    {
        // we can just extend our length!
        if (len < capacity())
        {
            *reinterpret_cast<T*>(ptr + (offset + len) * sizeof(T)) = t;
            ++len;
            return true;
        }
        // gotta realloc
        else
        {
            return false;
        }
    }

    template<typename T>
    void Array::remove(size_t idx)
    {
        if (idx >= len)
            throw std::out_of_range{ "index is out of range" };

        void* target = static_cast<char*>(ptr) + (offset + idx) * sizeof(T);
        std::memcpy(target, static_cast<char*>(target) + 1 * sizeof(T), (len - idx) * sizeof(T));
        --len;
    }
}

