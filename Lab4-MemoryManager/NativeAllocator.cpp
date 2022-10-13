#include <cstdlib>
#include <iostream>

#include "NativeAllocator.h"

void* NativeAllocator::alloc(size_t size)
{
    auto ptr = std::malloc(size);

#ifdef DEBUG
    m_Allocations.emplace(ptr, size);
#endif

    return ptr;
}

void NativeAllocator::free(void* ptr)
{
#ifdef DEBUG
    m_Allocations.extract(ptr);
#endif

    std::free(ptr);
}

#ifdef DEBUG
void NativeAllocator::dumpStat() const
{
    std::cout << "~~~Native Allocator~~~" << std::endl;
    std::cout << "Allocations count: " << m_Allocations.size() << std::endl;
}

void NativeAllocator::dumpBlocks() const
{
    for (auto [ptr, size] : m_Allocations)
    {
        std::cout << "Address: " << ptr << " | ";
        std::cout << "Size: " << size << std::endl;
    }
}

bool NativeAllocator::hasBusyBlocks() const
{
    return !m_Allocations.empty();
}
#endif