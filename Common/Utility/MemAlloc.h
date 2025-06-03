#pragma once
#include "Common/DataDefinitions.h"

class MemAlloc
{
public:
	static void* AllocMemory(size_t bytes);
	static void FillMemory(void* mem, size_t bytes, byte value);

	template<typename T> static T* AllocTMemory(size_t num)
	{
		return static_cast<T*>(AllocMemory(sizeof(T) * num));
	}
};