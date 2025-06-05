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
		IDrawable* Drawable = nullptr;
		D3D12_VERTEX_BUFFER_VIEW VertBufferView = {};
		ComPtr<ID3D12Resource> VertexBufferResource;
	};

	class RenderState : public IRenderState
	{
	public:
		RenderState(Framework* parentFramework);
		void Initialize(const RenderStateDesc& desc) override;
		void AddDrawable(IDrawable* drawable) override;
		void Execute() override;

		void RendererDraw(ID3D12GraphicsCommandList* commandList);

	private:
		void CreateRootSignature();

		void GenerateBufferViewAndResource(IDrawable& forDrawable, DrawableData& targetData);

		Framework* _framework;
		Renderer* _renderer;

		ComPtr<ID3D12RootSignature> _rootSignature;
		ComPtr<ID3D12PipelineState> _pipelineState;

		TDynamicList<DrawableData> _drawables;
	};
}
