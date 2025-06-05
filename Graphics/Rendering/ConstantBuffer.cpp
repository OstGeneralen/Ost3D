#include "../DX/DXCore.h"
#include "ConstantBuffer.h"

void ConstantBuffer::ReleaseBuffer()
{
	auto asResource = static_cast<ID3D12Resource*>(ResourcePtr);
	asResource->Unmap(0, nullptr);
	asResource->Release();
}