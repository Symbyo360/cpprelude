#include "cpprelude/platform.h"
#include <mutex>

#if defined(OS_WINDOWS)
#include <Windows.h>
#elif defined(OS_LINUX)
#include <sys/mman.h>
#endif

namespace cpprelude
{
    memory_context*
    global_memory_context()
    {
        static memory_context context;
        return &context;
    }

    namespace platform
    {
        void*
        virtual_alloc(void* address_hint, usize size)
        {
            if(size == 0)
                return nullptr;

            void* result = nullptr;

            #if defined(OS_WINDOWS)
                result = VirtualAlloc(address_hint, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            #elif defined(OS_LINUX)
                result = mmap(address_hint, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
            #endif

            return result;
        }

        bool
        virtual_free(void* ptr, usize size)
        {
            #if defined(OS_WINDOWS)
                return VirtualFree(ptr, size, MEM_RELEASE) != NULL;
            #elif defined(OS_LINUX)
                return munmap(ptr, size) == 0;
            #endif
        }
    }

    namespace helper
    {
        inline static std::mutex&
        print_lock()
        {
            static std::mutex _print_lock;
            return _print_lock;
        }

        void
        __acquire_print_lock()
        {
            print_lock().lock();
        }

        void
        __release_print_lock()
        {
            print_lock().unlock();
        }
    }
}