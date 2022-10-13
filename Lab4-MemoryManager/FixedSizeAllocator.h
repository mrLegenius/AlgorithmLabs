#pragma once
#include <vector>

class FixedSizeAllocator
{
public:
    FixedSizeAllocator();
    ~FixedSizeAllocator();
    void init(size_t blockSize, size_t blocksCount);
    void destroy();

    void* alloc();
    void free(void* ptr);
    bool contains(void* ptr);

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
    size_t block_size;
    void* start;
    size_t blocks_count;
    std::vector<bool> bits;

    bool try_get_free_block(int& index);
};
