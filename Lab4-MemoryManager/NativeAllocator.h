#pragma once
#include <cstdint>

#ifdef DEBUG
#include <unordered_map>
#endif

class NativeAllocator
{
public:
    NativeAllocator() = default;
    ~NativeAllocator() = default;
    void init() { };
    void destroy() { };
    void* alloc(size_t size);
    void free(void* ptr);

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
#ifdef DEBUG
std::unordered_map<void*, size_t> m_Allocations;
#endif
};
