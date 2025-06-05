#include <Graphics/DX/DXCore.h>

#include "Vertex.h"
#include <typeinfo>

StaticVertexInfo Vertex_Pos4_fCol4::StaticVertexFormatInfo()
{
	static D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA }
	};
	static StaticVertexInfo info{desc, 2, VertexFormats::fPos4_fCol4};
	return info;
}

vertexFormatID_t Vertex_Pos4_fCol4::StaticVertexFormatID()
{
	return VertexFormats::fPos4_fCol4;
}

vertexFormatID_t Vertex_Pos4_fCol4::VertexFormat() const
{
	return StaticVertexFormatID();
}
