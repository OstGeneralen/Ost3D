#include "MemAlloc.h"
#include <cstring>

void* MemAlloc::MemAllocMemory(size_t bytes)
{
    byte* mem = new byte[bytes];
    memset(mem, 0, bytes);
    return mem;
}

void MemAlloc::MemFillMemory(void* mem, size_t bytes, byte value)
{
    memset(mem, value, bytes);
}

void MemAlloc::MemReleaseMemory(unsigned char* mem, size_t bytes)
{
    delete[bytes] mem;
}
