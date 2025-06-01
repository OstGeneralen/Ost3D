#include "Utilities.h"
#include <cstring>

void Util::MemCopy(void* destination, const void* source, int size)
{
	memcpy_s(destination, size, source, size);
}
