#pragma once
#include "Common/DataDefinitions.h"

class MemAlloc
{
public:
	static void* AllocMemory(size_t bytes);
	static void FillMemory(void* mem, size_t bytes, byte value);

	static void ReleaseMemory(byte* mem, size_t bytes);

	template<typename T> static T* AllocTMemory(size_t num)
	{
		return static_cast<T*>(AllocMemory(sizeof(T) * num));
	}

	template<typename T> static void ReleaseTMemory(T* mem, size_t num)
	{
		void* asVPtr = mem;
		ReleaseMemory(static_cast<byte*>(asVPtr), num * sizeof(T));
	}
};