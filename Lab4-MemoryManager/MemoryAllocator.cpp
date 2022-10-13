#include "MemoryAllocator.h"
#include <cassert>

static constexpr int MaxManagedAlloc = 10 * 1024 * 1024;
static constexpr int FSABlockCount = 1024 * 1024;
static constexpr int CATotalMemory = 128 * 1024 * 1024;

static constexpr int Align = 8;

MemoryAllocator::~MemoryAllocator()
{
#ifdef DEBUG
    assert(!m_Initialized && "Memory allocator was not destroyed");
#endif
}

void MemoryAllocator::init()
{
#ifdef DEBUG
    assert(!m_Initialized && "Memory allocator is initialized already");
#endif

    fsa8.init(8, FSABlockCount);
    fsa16.init(16, FSABlockCount);
    fsa32.init(32, FSABlockCount);
    fsa64.init(64, FSABlockCount);
    fsa128.init(128, FSABlockCount);
    fsa256.init(256, FSABlockCount);
    fsa512.init(512, FSABlockCount);

    coalesce.init(CATotalMemory);
    native.init();

    m_Initialized = true;
}

void MemoryAllocator::destroy()
{
#ifdef DEBUG
    assert(m_Initialized && "Trying to destroy uninitialized memory allocator");
    assert(!hasBusyBlocks() && "There is used memory while trying to destroy allocator");
#endif

    fsa8.destroy();
    fsa16.destroy();
    fsa32.destroy();
    fsa64.destroy();
    fsa128.destroy();
    fsa256.destroy();
    fsa512.destroy();
    coalesce.destroy();
    native.destroy();

    m_Initialized = false;
}

void* MemoryAllocator::alloc(size_t size)
{
#ifdef DEBUG
    assert(m_Initialized && "Memory allocator is not initialized while trying to alloc");
#endif

    auto alignedSize = alignSize(size);

    if (alignedSize <= 8) return fsa8.alloc();
    if (alignedSize <= 16) return fsa16.alloc();
    if (alignedSize <= 32) return fsa32.alloc();
    if (alignedSize <= 64) return fsa64.alloc();
    if (alignedSize <= 128) return fsa128.alloc();
    if (alignedSize <= 256) return fsa256.alloc();
    if (alignedSize <= 512) return fsa512.alloc();

    if (alignedSize <= MaxManagedAlloc) return coalesce.alloc(alignedSize);

    return native.alloc(alignedSize);
}

void MemoryAllocator::free(void* p)
{
#ifdef DEBUG
    assert(m_Initialized && "Memory allocator is not initialized while trying to free");
#endif

    if (tryFreeFSA(fsa8, p)) return;
    if (tryFreeFSA(fsa16, p)) return;
    if (tryFreeFSA(fsa32, p)) return;
    if (tryFreeFSA(fsa64, p)) return;
    if (tryFreeFSA(fsa128, p)) return;
    if (tryFreeFSA(fsa256, p)) return;
    if (tryFreeFSA(fsa512, p)) return;
    if (tryFreeCoalesce(coalesce, p)) return;
    native.free(p);
}

#ifdef DEBUG
void MemoryAllocator::dumpStat() const
{
    assert(m_Initialized && "Memory allocator is not initialized while trying to dumpStat");

    fsa8.dumpStat();
    fsa16.dumpStat();
    fsa32.dumpStat();
    fsa64.dumpStat();
    fsa128.dumpStat();
    fsa256.dumpStat();
    fsa512.dumpStat();
    coalesce.dumpStat();
    native.dumpStat();
}

void MemoryAllocator::dumpBlocks() const
{
    assert(m_Initialized && "Memory allocator is not initialized while trying to dumpBlocks");

    fsa8.dumpBlocks();
    fsa16.dumpBlocks();
    fsa32.dumpBlocks();
    fsa64.dumpBlocks();
    fsa128.dumpBlocks();
    fsa256.dumpBlocks();
    fsa512.dumpBlocks();
    coalesce.dumpBlocks();
    native.dumpBlocks();
}

bool MemoryAllocator::hasBusyBlocks() const
{
    return fsa8.hasBusyBlocks()
           || fsa16.hasBusyBlocks()
           || fsa32.hasBusyBlocks()
           || fsa64.hasBusyBlocks()
           || fsa128.hasBusyBlocks()
           || fsa256.hasBusyBlocks()
           || fsa512.hasBusyBlocks()
           || coalesce.hasBusyBlocks()
           || native.hasBusyBlocks();
}
#endif

size_t MemoryAllocator::alignSize(size_t size)
{
    auto alignedSize = size;

    auto needAlignment = size % Align != 0;
    if (needAlignment)
    {
        auto alignment = Align - size % Align;
        alignedSize += alignment;
    }

    return alignedSize;
}

bool MemoryAllocator::tryFreeFSA(FixedSizeAllocator& fsa, void* ptr)
{
    if (!fsa.contains(ptr)) return false;

    fsa.free(ptr);
    return true;
}

bool MemoryAllocator::tryFreeCoalesce(CoalesceAllocator& coalesce, void* ptr)
{
    if (!coalesce.contains(ptr)) return false;

    coalesce.free(ptr);
    return true;
}

