#include <cstdlib>
#include <iostream>

#include "CoalesceAllocator.h"

void CoalesceAllocator::init(size_t heapSize)
{
    start = std::malloc(heapSize);
    m_HeapSize = heapSize;

    auto header = reinterpret_cast<Boundary*>(start);
    header->SetSize(heapSize);
    header->SetStatusFlag(false);
}

void CoalesceAllocator::destroy()
{
    std::free(start);
}

void* CoalesceAllocator::alloc(size_t size)
{
    auto blockSize = size + sizeof(Boundary) * 2;
    if (blockSize < MinBlockSize)
        blockSize = MinBlockSize;

    if (!start) return nullptr;

    auto headerPtr = reinterpret_cast<Boundary*>(start);

    while (headerPtr)
    {
        if (!headerPtr->GetStatusFlag() && headerPtr->GetSize() >= blockSize)
        {
            auto initialBlockSize = headerPtr->GetSize();
            auto leftBlockSize = initialBlockSize - blockSize;

            if (leftBlockSize < MinBlockSize)
                blockSize = initialBlockSize;

            SetBlock(headerPtr, blockSize, true);

            auto needDivideBlock = blockSize != initialBlockSize;
            if (!needDivideBlock)
                return reinterpret_cast<char*>(headerPtr) + sizeof(Boundary);

            auto nextHeader = GetNextHeader(headerPtr);

            if (nextHeader)
                SetBlock(nextHeader, leftBlockSize, false);

            return reinterpret_cast<char*>(headerPtr) + sizeof(Boundary);
        }

        headerPtr = GetNextHeader(headerPtr);
    }

    return nullptr;
}

void CoalesceAllocator::free(void* p)
{
    auto headerPtr = reinterpret_cast<Boundary*>(reinterpret_cast<char*>(p) - sizeof(Boundary));
    headerPtr->SetStatusFlag(false);

    //Next coalesce
    auto nextHeader = GetNextHeader(headerPtr);
    if (nextHeader && !nextHeader->GetStatusFlag())
        Coalesce(headerPtr, nextHeader);

    //Previous Coalesce
    auto previousHeader = GetPreviousHeader(headerPtr);
    if (previousHeader && !previousHeader->GetStatusFlag())
        Coalesce(previousHeader, headerPtr);
}

bool CoalesceAllocator::contains(void* ptr) const
{
    const auto ptrAddress = reinterpret_cast<char*>(ptr);
    const auto startAddress = reinterpret_cast<char*>(start);
    const auto endAddress = startAddress + m_HeapSize;

    return startAddress <= ptrAddress && ptrAddress < endAddress;
}

#ifdef DEBUG

void CoalesceAllocator::dumpStat() const
{
    std::cout << "~~~Coalesce Allocator~~~" << std::endl;
    auto headerPtr = reinterpret_cast<Boundary*>(start);

    size_t freeBlocks = 0;
    size_t busyBlocks = 0;

    while (headerPtr)
    {
        auto isFree = !headerPtr->GetStatusFlag();

        auto freeText = isFree ? "Free" : "Busy";
        std::cout << freeText << " block: | ";
        std::cout << "Size: " << headerPtr->GetSize() << std::endl;

        if (isFree)
            freeBlocks++;
        else
            busyBlocks++;

        headerPtr = GetNextHeader(headerPtr);
    }

    std::cout << "Blocks reserved: " << freeBlocks + busyBlocks << " | ";

    if (freeBlocks + busyBlocks == 0) return;

    std::cout << "Free blocks: " << freeBlocks << " | ";
    std::cout << "Busy blocks: " << busyBlocks << std::endl;
}

void CoalesceAllocator::dumpBlocks() const
{
    auto headerPtr = reinterpret_cast<Boundary*>(start);

    while (headerPtr)
    {
        if (headerPtr->GetStatusFlag())
        {
            std::cout << "Address: " << headerPtr << " | ";
            std::cout << "Size: " << headerPtr->GetSize() << std::endl;
        }

        headerPtr = GetNextHeader(headerPtr);
    }
}

bool CoalesceAllocator::hasBusyBlocks() const
{
    auto headerPtr = reinterpret_cast<Boundary*>(start);

    while (headerPtr)
    {
        if (headerPtr->GetStatusFlag())
            return true;

        headerPtr = GetNextHeader(headerPtr);
    }

    return false;
}

#endif

void CoalesceAllocator::Coalesce(Boundary* leftHeader, const Boundary* rightHeader)
{
    auto blockSize = leftHeader->GetSize();
    auto nextBlockSize = rightHeader->GetSize();
    auto newSize = blockSize + nextBlockSize;

    SetBlock(leftHeader, newSize, false);
}

void CoalesceAllocator::SetBlock(Boundary* headerPtr, size_t blockSize, bool busy)
{
    headerPtr->SetStatusFlag(busy);
    headerPtr->SetSize(blockSize);

    auto footerPtr = GetCurrentFooter(headerPtr);
    footerPtr->SetStatusFlag(busy);
    footerPtr->SetSize(blockSize);
}

CoalesceAllocator::Boundary* CoalesceAllocator::GetNextHeader(Boundary* currentHeaderPtr) const
{
    auto nextBoundaryPtr = reinterpret_cast<char*>(currentHeaderPtr) + currentHeaderPtr->GetSize();

    if (!contains(nextBoundaryPtr))
        return nullptr;

    return reinterpret_cast<Boundary*>(nextBoundaryPtr);
}

CoalesceAllocator::Boundary* CoalesceAllocator::GetCurrentFooter(Boundary* currentHeaderPtr) const
{
    auto nextBoundaryPtr = reinterpret_cast<char*>(currentHeaderPtr) + currentHeaderPtr->GetSize();

    nextBoundaryPtr -= sizeof(Boundary);

    if (!contains(nextBoundaryPtr))
        return nullptr;

    return reinterpret_cast<Boundary*>(nextBoundaryPtr);
}

CoalesceAllocator::Boundary* CoalesceAllocator::GetPreviousHeader(Boundary* currentHeaderPtr) const
{
    auto previousFooterPtr = reinterpret_cast<char*>(currentHeaderPtr) - sizeof(Boundary);
    auto previousFooter = reinterpret_cast<Boundary*>(previousFooterPtr);

    auto previousHeaderPtr = reinterpret_cast<char*>(currentHeaderPtr) - previousFooter->GetSize();

    if (!contains(previousHeaderPtr))
        return nullptr;

    return reinterpret_cast<Boundary*>(previousHeaderPtr);
}

size_t CoalesceAllocator::Boundary::GetSize() const
{
    return m_Size >> 1;
}

void CoalesceAllocator::Boundary::SetSize(size_t size)
{
    auto flag = GetStatusFlag();
    m_Size = size << 1;
    SetStatusFlag(flag);
}

void CoalesceAllocator::Boundary::SetStatusFlag(bool state)
{
    m_Size = (m_Size & ~(1UL << bit)) | (state << bit);
}

bool CoalesceAllocator::Boundary::GetStatusFlag() const
{
    return (m_Size >> bit) & 1U;
}
