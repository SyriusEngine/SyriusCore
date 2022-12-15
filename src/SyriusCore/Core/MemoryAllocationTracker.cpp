#include "MemoryAllocationTracker.hpp"

#if defined(SR_CORE_DEBUG)

namespace Syrius{

    Size MemoryAllocationTracker::m_AllocatedMemory = 0;
    Size MemoryAllocationTracker::m_FreedMemory = 0;
    Size MemoryAllocationTracker::m_Usage = 0;

    void MemoryAllocationTracker::allocate(Size size) {
        m_AllocatedMemory += size;
        m_Usage += size;
    }

    void MemoryAllocationTracker::deallocate(Size size) {
        m_FreedMemory += size;
        m_Usage -= size;
    }

    Size MemoryAllocationTracker::getTotalAllocated() {
        return m_AllocatedMemory;
    }

    Size MemoryAllocationTracker::getTotalFreed() {
        return m_FreedMemory;
    }

    Size MemoryAllocationTracker::getUsage() {
        return m_Usage;
    }
}


void* operator new(size_t size){
    Syrius::MemoryAllocationTracker::allocate(size);
    return malloc(size);
}

void operator delete(void* memory, size_t size) {
    Syrius::MemoryAllocationTracker::deallocate(size);
    free(memory);
}

#endif
