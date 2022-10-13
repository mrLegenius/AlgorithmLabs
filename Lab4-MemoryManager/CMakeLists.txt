cmake_minimum_required(VERSION 3.23)
project(MemoryManager)
add_compile_definitions(DEBUG)

set(CMAKE_CXX_STANDARD 17)

add_executable(MemoryManager
        main.cpp
        MemoryAllocator.cpp
        FixedSizeAllocator.cpp
        CoalesceAllocator.cpp
        NativeAllocator.cpp)
