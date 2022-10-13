#pragma once
#include <iostream>
#include <vector>
#include "FixedSizeAllocator.h"
#include "CoalesceAllocator.h"
#include "NativeAllocator.h"

class MemoryAllocator
{
public:
    MemoryAllocator() = default;
    virtual ~MemoryAllocator();
    virtual void init();
    virtual void destroy();
    virtual void *alloc(size_t size);
    virtual void free(void* p);

#ifdef DEBUG
    virtual void dumpStat() const;
    virtual void dumpBlocks() const;
    [[nodiscard]] bool hasBusyBlocks() const;
#else
    virtual void dumpStat() const { };
    virtual void dumpBlocks() const { };
    [[nodiscard]] bool hasBusyBlocks() const { return false; }
#endif

private:
    bool m_Initialized = false;

    FixedSizeAllocator fsa8{};
    FixedSizeAllocator fsa16{};
    FixedSizeAllocator fsa32{};
    FixedSizeAllocator fsa64{};
    FixedSizeAllocator fsa128{};
    FixedSizeAllocator fsa256{};
    FixedSizeAllocator fsa512{};

    CoalesceAllocator coalesce{};

    NativeAllocator native{};

    static size_t alignSize(size_t size);
    static inline bool tryFreeFSA(FixedSizeAllocator& fsa, void* ptr);
    static inline bool tryFreeCoalesce(CoalesceAllocator& coalesce, void* ptr);
};