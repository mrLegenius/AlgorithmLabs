#include <cstdlib>
#include <iostream>

#include "FixedSizeAllocator.h"

FixedSizeAllocator::FixedSizeAllocator()
{

}

FixedSizeAllocator::~FixedSizeAllocator()
{

}

void FixedSizeAllocator::init(size_t blockSize, size_t blocksCount)
{
    block_size = blockSize;
    blocks_count = blocksCount;
    bits.resize(blocksCount);
    start = std::malloc(blocksCount * blockSize);
}

void FixedSizeAllocator::destroy()
{
    std::free(start);
}

void* FixedSizeAllocator::alloc()
{
    int index = 0;
    if (!try_get_free_block(index))
        return nullptr;

    bits[index] = true;

    return reinterpret_cast<void*>(reinterpret_cast<char*>(start) + block_size * index);
}

void FixedSizeAllocator::free(void* ptr)
{
    const size_t index = (reinterpret_cast<char*>(ptr) - reinterpret_cast<char*>(start)) / block_size;
    bits[index] = false;
}

bool FixedSizeAllocator::contains(void* ptr)
{
    const auto ptrAddress = reinterpret_cast<char*>(ptr);
    const auto startAddress = reinterpret_cast<char*>(start);
    const auto endAddress = startAddress + blocks_count * block_size;

    return startAddress <= ptrAddress && ptrAddress <= endAddress;
}

bool FixedSizeAllocator::try_get_free_block(int& index)
{
    for (int i = 0; i < bits.size(); ++i)
    {
        if (bits.at(i)) continue;

        index = i;
        return true;
    }

    return false;
}

#ifdef DEBUG

void FixedSizeAllocator::dumpStat() const
{
    std::cout << "~~~Fixed Size Allocator " << block_size << "~~~" << std::endl;

    size_t freeBlocks = 0;
    for (unsigned int i = 0; i < blocks_count; i++)
    {
        if (!bits[i])
            freeBlocks++;
    }

    std::cout << "Blocks reserved: " << blocks_count << " | ";
    std::cout << "Free blocks: " << freeBlocks << " | ";
    std::cout << "Busy blocks: " << (blocks_count - freeBlocks) << std::endl;
}

void FixedSizeAllocator::dumpBlocks() const
{
	for (unsigned int i = 0; i < blocks_count; i++)
    {
		if (!bits[i]) continue;

        const auto startAddress = reinterpret_cast<char*>(start);
        const auto ptrAddress = reinterpret_cast<void*>(startAddress + i * block_size);
        std::cout << "Address: " << ptrAddress << " | ";
        std::cout << "Size: " << block_size << std::endl;
	}
}

bool FixedSizeAllocator::hasBusyBlocks() const
{
    for (unsigned int i = 0; i < blocks_count; i++)
        if (bits[i]) return true;

    return false;
}

#endif
