#pragma once
#include <Common/DataDefinitions.h>

struct MemAlloc
{
	static void* MemAllocMemory(size_t bytes);
	static void MemFillMemory(void* mem, size_t bytes, byte value);

	static void MemReleaseMemory(byte* mem, size_t bytes);

	template<typename T> static T* AllocTMemory(size_t num)
	{
		return static_cast<T*>(MemAllocMemory(sizeof(T) * num));
	}

	template<typename T> static void ReleaseTMemory(T* mem, size_t num)
	{
		void* asVPtr = mem;
		MemReleaseMemory(static_cast<byte*>(asVPtr), num * sizeof(T));
	}
};