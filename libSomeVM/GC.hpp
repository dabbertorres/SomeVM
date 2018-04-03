#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <list>
#include <mutex>
#include <thread>

namespace svm
{
    class Registry;

    class GC
    {
    public:
        GC(const Registry& reg, std::chrono::milliseconds collectPeriod);
        ~GC();

        void start();
        void pause();
        void resume();
        void stop();

        size_t memUsage() const;
        size_t maxMemUsage() const;
        size_t currentAllocs() const;
        size_t totalAllocs() const;
        size_t totalFrees() const;

        void* alloc(size_t size);
        void* realloc(void* userptr, size_t size);
        void free(void* userptr);

        class AllocationError : public std::bad_alloc
        {
            using std::bad_alloc::bad_alloc;
        };

        class Handle
        {
            friend class GC;
        public:
            size_t capacity() const;

        protected:
            Handle(GC& gc, size_t size);

            // for allocating objects outside of the GC
            Handle(std::nullptr_t);

            // the GC will be doing the free'ing
            ~Handle() = default;

        protected:
            void* ptr;
        };

    private:
        // runs in a separate thread doing the actual GC'ing
        void collect();

        const Registry& registry;

        std::list<void*> allocs;
        size_t allocCount;
        size_t freeCount;
        size_t totalMemUsage;

        std::thread collectThread;
        std::chrono::milliseconds collectPeriod;
        std::atomic<bool> collectRun;
        std::atomic<bool> collectPause;
        std::condition_variable collectSignal;
        mutable std::mutex collectMutex;

        using LockType = std::unique_lock<decltype(collectMutex)>;
    };
}
