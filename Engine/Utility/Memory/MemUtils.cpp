#include "MemUtils.h"
#include <cstring>


void ost::MemCopy(void* destination, const void* source, int size)
{
	memcpy_s(destination, size, source, size);
}

void* ost::AllocMem(size_t bytes)
{
    byte* mem = new byte[bytes];
    memset(mem, 0, bytes);
    return mem;
}

void ost::FillMem(void* mem, size_t bytes, byte value)
{
    memset(mem, value, bytes);
}

void ost::ReleaseMem(unsigned char* mem, size_t bytes)
{
    delete[bytes] mem;
}