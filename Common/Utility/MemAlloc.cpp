#include "MemAlloc.h"
#include <cstring>

void* MemAlloc::AllocMemory(size_t bytes)
{
    byte* mem = new byte[bytes];
    memset(mem, 0, bytes);
    return mem;
}

void MemAlloc::FillMemory(void* mem, size_t bytes, byte value)
{
    memset(mem, value, bytes);
}

void MemAlloc::ReleaseMemory(byte* mem, size_t bytes)
{
    delete[bytes] mem;
}
