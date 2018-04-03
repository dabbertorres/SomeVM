#include "GC.hpp"

#include <algorithm>
#include <numeric>
#include <vector>

#include "Registry.hpp"

namespace svm
{
    GC::GC(const Registry& reg, std::chrono::milliseconds collectPeriod)
        : registry{ reg },
        allocs{},
        allocCount{ 0 },
        freeCount{ 0 },
        totalMemUsage{ 0 },
        collectPeriod{ collectPeriod }
    {}

    GC::~GC()
    {
        for (auto ptr : allocs)
            std::free(ptr);
    }

    void GC::start()
    {
        collectRun = true;
        collectThread = std::thread{ &GC::collect, this };
    }

    void GC::pause()
    {
        collectPause = true;
    }

    void GC::resume()
    {
        collectPause = false;
        collectSignal.notify_one();
    }

    void GC::stop()
    {
        collectSignal.notify_one();
        collectRun = false;

        if (collectThread.joinable())
            collectThread.join();
    }

    size_t GC::memUsage() const
    {
        return std::reduce(allocs.begin(), allocs.end(), 0ull, [](size_t s, void* userptr) { return s + *reinterpret_cast<size_t*>(static_cast<char*>(userptr) - sizeof(size_t)); });
    }

    size_t GC::maxMemUsage() const
    {
        return totalMemUsage;
    }

    size_t GC::currentAllocs() const
    {
        return allocs.size();
    }

    size_t GC::totalAllocs() const
    {
        return allocCount;
    }

    size_t GC::totalFrees() const
    {
        return freeCount;
    }

    void* GC::alloc(size_t size)
    {
        void* ptr = std::malloc(size + sizeof(size_t));
        *static_cast<size_t*>(ptr) = size;
        void* userptr = static_cast<char*>(ptr) + sizeof(size_t);
        allocs.push_back(userptr);
        ++allocCount;
        totalMemUsage += size + sizeof(size_t);
        return userptr;
    }

    void* GC::realloc(void* userptr, size_t size)
    {
        void* ptr = static_cast<char*>(userptr) - sizeof(size_t);
        size_t prevSize = *reinterpret_cast<size_t*>(ptr);

        // if we're already big enough, no need to do anything
        if (size <= prevSize)
            return userptr;

        void* newPtr = std::realloc(ptr, size + sizeof(size_t));
        *static_cast<size_t*>(newPtr) = size;

        // check if the OS actually did allocate new space, and update accordingly
        if (newPtr != ptr)
        {
            allocs.erase(std::remove(allocs.begin(), allocs.end(), userptr));
            ++freeCount;
            ++allocCount;
        }

        totalMemUsage += size - prevSize;

        return static_cast<char*>(newPtr) + sizeof(size_t);
    }

    void GC::free(void* userptr)
    {
        void* ptr = static_cast<char*>(userptr) - sizeof(size_t);
        std::free(ptr);
        allocs.erase(std::remove(allocs.begin(), allocs.end(), userptr));
        ++freeCount;
    }

    void GC::collect()
    {
        while (collectRun)
        {
            LockType lock{ collectMutex };

            // get all types that allocate memory
            std::vector<Value> objs;
            std::copy_if(registry.begin(), registry.end(), std::back_inserter(objs),
                         [](auto v)
                         {
                             return std::holds_alternative<String>(v) || std::holds_alternative<Array>(v) || std::holds_alternative<Object>(v);
                         });

            // get all pointers that are NOT referenced in objs
            std::vector<void*> toFree;
            std::remove_copy_if(allocs.begin(), allocs.end(), std::back_inserter(toFree),
                                [&](auto ptr)
                                {
                                    return std::any_of(objs.begin(), objs.end(),
                                                       [&](auto v)
                                                       {
                                                           auto idx = v.index();
                                                           Handle* h;
                                                           switch (idx)
                                                           {
                                                           case typeIndex<String>():
                                                               h = &std::get<String>(v);
                                                               break;

                                                           case typeIndex<Array>():
                                                               h = &std::get<Array>(v);
                                                               break;

                                                           case typeIndex<Object>():
                                                               h = &std::get<Object>(v);
                                                               break;

                                                           default:
                                                               return false;
                                                           }

                                                           return h->ptr == ptr;
                                                       });
                                });

            // free what's left over!
            std::for_each(toFree.begin(), toFree.end(),
                          [](void* userptr)
                          {
                              void* ptr = static_cast<char*>(userptr) - sizeof(size_t);
                              std::free(ptr);
                          });

            bool wait = false;
            do
                wait = collectSignal.wait_for(lock, collectPeriod, [&]() -> bool { return collectPause; });
            while (wait);
        }
    }

    size_t GC::Handle::capacity() const
    {
        return *reinterpret_cast<size_t*>(static_cast<char*>(ptr) - sizeof(size_t));
    }

    GC::Handle::Handle(GC& gc, size_t size)
        : ptr{ gc.alloc(size) }
    {}

    GC::Handle::Handle(std::nullptr_t)
        : ptr{ nullptr }
    {}
}
