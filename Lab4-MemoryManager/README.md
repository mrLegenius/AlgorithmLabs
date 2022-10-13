# Memory Allocator
Made for Algorimth analysis course in ITMO 2022 Gamedev.

## General
This is a universal allocator based on different approaches.
Every allocation is based on first fit idea.

#### Fixed sized allocators
It is used for allocations with same block size.
There is several different block size allocators:
8, 16, 32, 64, 128, 256, 512 bytes.

This allocator consists of 2 elements: Memory pool and Vector of bools.
Vector is optimized for keeping bool. So there is only one extra byte for every 8 reserved block.

Algorithm is searching for the first empty block in pool.

Pros:
* Less fragmentation.

Cons:
* Extra memory for bool vector.
* Linear time block access.

It can be optimized in future by changing to linked list structure.

#### Coalesce allocator with free list
Allocates variable size blocks.
It used for allocations greater then 512 bytes or less then 10 Mb.
Each block size consists of header, payload and footer.

Header and footer is 8 bytes each.

They are keeping info about block: it's size and state (1 bit is pinched off from size). 

When allocating a block it searches for first fit with enough space for request. Then it tries to split block if it is possible.

When deallocating a memory allocator tries to сombine together first next block and then previous block

Pros:
* Less fragmentation.

Cons:
* Linear time block access.

It can be optimized in future by changing to explicit free list method.

#### Native allocator

This allocator is using standart allocation methods if requested memory is equal or greater then 10 Mb.

## Interface

#### MemoryAllocator();
Creates uninitialized memory manager

#### ~MemoryAllocator();
Check if all block are freed

#### void init();
Requests memory for proper work of allocators
 
#### void destroy();
Return all memory to OS

#### void *alloc(size_t size);
Allocates requests memory with aligning to 8 bytes

#### void free(void *p);
Free previously requested memory

#### void dumpStat() const;
Prints number of free and busy blocks. Also prints list of all blocks

#### void dumpBlocks() const;
Prints address and size of all busy blocks
