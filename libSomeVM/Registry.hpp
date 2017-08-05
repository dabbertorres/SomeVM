#pragma once

#include <cstdint>
#include <vector>
#include <list>

namespace sl
{
    class Object
    {
    public:
        Object();

        Object(const Object& other);
        Object& operator=(const Object& other);

        Object(Object&& other);
        Object& operator=(Object&& other);

        ~Object();

        void ref();
        void unref();
        size_t refCount() const;

    private:
        size_t sizeBytes;
        size_t references;
        void* pointer;
    };

    // may contain a value, or an object pointer
    using Register = std::uint64_t;

    class Registry
    {
    public:
        Registry(size_t size);
        
        // delete?
        Registry(const Registry& other);
        Registry& operator=(const Registry& other);

        Registry(Registry&& other);
        Registry& operator=(Registry&& other);

        ~Registry();

        template<typename T>
        T get(size_t index) const;

        Object* get(size_t index, const Heap& heap) const;

    private:
        std::vector<Register> values;
    };

    class Heap
    {
    public:
        Heap(size_t bytes);

        // delete?
        Heap(const Heap& other);
        Heap& operator=(const Heap& other);

        Heap(Heap&& other);
        Heap& operator=(Heap&& other);

        ~Heap();



    private:
        std::list<Object> objects;
        void* pointer;
    };
}
