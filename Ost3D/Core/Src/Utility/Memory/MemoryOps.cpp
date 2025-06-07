// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#include "Public/Utility/Memory/MemoryOps.h"

#include <cstring>

// ------------------------------------------------------------

void ost::memory::MemCopy(void* destination, const void* source, uint64 size)
{
    memcpy_s(destination, size, source, size);
}

// ------------------------------------------------------------

void* ost::memory::AllocMem(size_t bytes)
{
    byte* mem = new byte[bytes];
    memset(mem, 0, bytes);
    return mem;
}

// ------------------------------------------------------------

void ost::memory::FillMem(void* mem, size_t bytes, byte value)
{
    memset(mem, value, bytes);
}

// ------------------------------------------------------------

void ost::memory::ReleaseMem(unsigned char* mem, size_t bytes)
{
    delete[bytes] mem;
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------