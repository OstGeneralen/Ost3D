#pragma once
#include <Common/Utility/Utilities.h>

struct ConstantBuffer
{
	template<typename TBufferData>
	void CopyAcross(const TBufferData& data)
	{
		Util::MemCopy(BufferAddress, &data, sizeof(TBufferData));
	}

	void ReleaseBuffer();

	void* BufferAddress;
	void* ResourcePtr;
};