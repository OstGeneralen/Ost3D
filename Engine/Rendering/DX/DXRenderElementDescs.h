#pragma once
#include <Engine/Rendering/Drawable/Vertex_PosCol.h>
#include <Engine/Rendering/DX/DXCore.h>

namespace ost
{
	namespace dx
	{
		struct DXRenderElementDescs
		{
			template<typename TVertexType> static D3D12_INPUT_ELEMENT_DESC* GetDesc(unsigned& numElements) { numElements = 0; return nullptr; }

			template<> static D3D12_INPUT_ELEMENT_DESC* GetDesc<Vertex_Pos32f_Col32f>(unsigned& numElements)
			{
				static D3D12_INPUT_ELEMENT_DESC descs[] = {
					{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
					{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
				};

				numElements = 2;
				return descs;
			}
		};
	}
}