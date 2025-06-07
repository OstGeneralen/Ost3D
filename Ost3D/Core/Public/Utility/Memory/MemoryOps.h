// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#pragma once
#include "Ost3D/Core/Public/TypeDefs.h"

// ------------------------------------------------------------

namespace ost
{
	namespace memory
	{
		extern void MemCopy(void* destination, const void* source, uint64 size);

		template<typename T>
		void MemCopyT(T* destination, const T* source, uint64 num)
		{
			MemCopy(destination, source, sizeof(T) * num);
		}

		extern void* AllocMem(size_t bytes);
		extern void FillMem(void* mem, size_t bytes, byte value);

		extern void ReleaseMem(byte* mem, uint64 bytes);

		template<typename T> static T* AllocMemT(uint64 num)
		{
			return static_cast<T*>(AllocMem(sizeof(T) * num));
		}

		template<typename T> static void ReleaseMemT(T* mem, uint64 num)
		{
			void* asVPtr = mem;
			ReleaseMem(static_cast<byte*>(asVPtr), num * sizeof(T));
		}
	}
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------