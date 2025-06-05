#pragma once
#include "DXCore.h"
#include "../Rendering/RenderState.h"
#include <Common/Container/DynamicList.h>

namespace DX
{
	class Framework;
	class Renderer;

	struct DrawableData
	{
		Drawable Instance;
		D3D12_VERTEX_BUFFER_VIEW VertBufferView = {};
		ComPtr<ID3D12Resource> VertexBufferResource;
	};

	class RenderState : public IRenderState
	{
	public:
		RenderState(Framework* parentFramework, const RenderStateDesc& desc);
		void Initialize(const RenderStateDesc& desc);
		bool AddDrawable(const Drawable& drawable) override;
		void SetConstantBuffer(ConstantBuffer* buffer ) override;

		void RendererDraw(ID3D12GraphicsCommandList* commandList);

	private:
		void CreateRootSignature(const RenderStateDesc& desc);

		void GenerateBufferViewAndResource(const Drawable& forDrawable, DrawableData& targetData);

		Framework* _framework;
		Renderer* _renderer;

		ComPtr<ID3D12RootSignature> _rootSignature;
		ComPtr<ID3D12PipelineState> _pipelineState;

		TDynamicList<DrawableData> _drawables;

		vertexFormatID_t _validVertexFormatID;

		ConstantBuffer* _cBufPtr{ nullptr };
	};
}
