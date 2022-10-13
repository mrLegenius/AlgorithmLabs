#include "MemoryAllocator.h"

int main()
{
    MemoryAllocator allocator{};
    allocator.init();

    int *pi = (int *)allocator.alloc(sizeof(int));
    double *pd = (double *)allocator.alloc(sizeof(double));
    int *pa = (int *)allocator.alloc(10 * sizeof(int));
    double *darray = (double *)allocator.alloc(1024 * sizeof(double));
    int *array = (int *)allocator.alloc(512 * sizeof(int));
    int *array2 = (int *)allocator.alloc(512 * sizeof(int));
    int *bigArray = (int *)allocator.alloc(1024 * 1024 * 20 * sizeof(int));

#ifdef DEBUG
    allocator.dumpStat();
    allocator.dumpBlocks();
#endif

    allocator.free(pi);
    allocator.free(pa);
    allocator.free(pd);
    allocator.free(darray);
    allocator.free(array);
    allocator.free(array2);
    allocator.free(bigArray);

#ifdef DEBUG
    allocator.dumpStat();
#endif

    allocator.destroy();
    return 0;
}
