#pragma once
#include <cstdint>

class CoalesceAllocator
{
public:
    struct Boundary
    {
    private:
        static constexpr size_t bit = 0;

    public:
        [[nodiscard]] size_t GetSize() const;
        void SetSize(size_t size);
        void SetStatusFlag(bool state);
        [[nodiscard]] bool GetStatusFlag() const;
    private:
        size_t m_Size = 0;
    };
    static constexpr int MinBlockSize = sizeof(Boundary) + sizeof(uintptr_t) + sizeof(Boundary);

public:
    CoalesceAllocator() = default;
    ~CoalesceAllocator() = default;
    void init(size_t heapSize);
    void destroy();
    void *alloc(size_t size);
    void free(void* p);
    bool contains(void* ptr) const;

#ifdef DEBUG
    void dumpStat() const;
    void dumpBlocks() const;
    [[nodiscard]] bool hasBusyBlocks() const;
#else
    void dumpStat() const { };
    void dumpBlocks() const { };
    [[nodiscard]] bool hasBusyBlocks() const { return false; }
#endif



private:
    void* start;
    size_t m_HeapSize;

    void SetBlock(Boundary* headerPtr, size_t blockSize, bool busy);
    [[nodiscard]] Boundary* GetNextHeader(Boundary* currentHeaderPtr) const;
    [[nodiscard]] Boundary* GetCurrentFooter(Boundary* currentHeaderPtr) const;
    [[nodiscard]] Boundary* GetPreviousHeader(Boundary* currentHeaderPtr) const;

    void Coalesce(Boundary* headerPtr, const Boundary* nextHeader);
};
