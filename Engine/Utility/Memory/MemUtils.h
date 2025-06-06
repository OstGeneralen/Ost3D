#pragma once
#include <Engine/OstEngineTypedefs.h>

namespace ost
{
	extern void MemCopy(void* destination, const void* source, int size);

	template<typename T>
	void MemCopyT(T* destination, const T* source, size_t num)
	{
		MemCopy(destination, source, sizeof(T) * num);
	}

	extern void* AllocMem(size_t bytes);
	extern void FillMem(void* mem, size_t bytes, byte value);

	extern void ReleaseMem(byte* mem, size_t bytes);

	template<typename T> static T* AllocMemT(size_t num)
	{
		return static_cast<T*>(AllocMem(sizeof(T) * num));
	}

	template<typename T> static void ReleaseMemT(T* mem, size_t num)
	{
		void* asVPtr = mem;
		ReleaseMem(static_cast<byte*>(asVPtr), num * sizeof(T));
	}
}