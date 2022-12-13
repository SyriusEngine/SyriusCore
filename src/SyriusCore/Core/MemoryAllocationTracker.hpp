#pragma once

#include "../../../include/SyriusCore/Core/SyriusCoreInclude.hpp"

#if defined(SR_CORE_DEBUG)

namespace Syrius{

    class MemoryAllocationTracker{
    public:
        MemoryAllocationTracker() = delete;

        static void allocate(Size size);

        static void deallocate(Size size);

        static Size getTotalAllocated();

        static Size getTotalFreed();

        static Size getUsage();

    private:
        static Size m_AllocatedMemory;
        static Size m_FreedMemory;
        static Size m_Usage;


    };

}

void* operator new(size_t size);

void operator delete(void* memory, size_t size);

#endif