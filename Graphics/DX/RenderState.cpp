#include "RenderState.h"
#include "Framework.h"
#include "Renderer.h"
#include <Common/Utility/Utilities.h>

DX::RenderState::RenderState(Framework* parentFramework, const RenderStateDesc& desc)
	: _framework(parentFramework)
	, _renderer(static_cast<Renderer*>(&(_framework->GetRenderer())))
	//, _drawables(10)
{
	CreateRootSignature(desc);

}

void DX::RenderState::Initialize(const RenderStateDesc& desc)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	psoDesc.InputLayout = { static_cast<D3D12_INPUT_ELEMENT_DESC*>(desc.VertexFormatInfo.InputElementsDesc), desc.VertexFormatInfo.NumInputElements };
	psoDesc.pRootSignature = _rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(desc.VertexShader.CompiledData, desc.VertexShader.CompiledDataSize);
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(desc.PixelShader.CompiledData, desc.PixelShader.CompiledDataSize);
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	psoDesc.DepthStencilState.DepthEnable = true;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
	psoDesc.DepthStencilState.StencilEnable = false;
	
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;

	_framework->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_pipelineState));

	_validVertexFormatID = desc.VertexFormatInfo.VertexFormatID;
}

bool DX::RenderState::AddDrawable(const Drawable& drawable)
{
	if (drawable.VertexFormatID != _validVertexFormatID)
	{
		return false;
	}

	DrawableData data{};
	data.Instance = drawable;
	GenerateBufferViewAndResource(drawable, data);
	_drawables.Emplace(data);
}

void DX::RenderState::SetConstantBuffer(ConstantBuffer* buffer)
{
	_cBufPtr = buffer;
}

void DX::RenderState::RendererDraw(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetGraphicsRootSignature(_rootSignature.Get());
	commandList->SetPipelineState(_pipelineState.Get());

	if (_cBufPtr != nullptr)
	{
		auto asResource = static_cast<ID3D12Resource*>(_cBufPtr->ResourcePtr);
		commandList->SetGraphicsRootConstantBufferView(0, asResource->GetGPUVirtualAddress());
	}

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (size_t dIdx = 0; dIdx < _drawables.Size(); ++dIdx)
	{
		commandList->IASetVertexBuffers(0, 1, &(_drawables[dIdx].VertBufferView));
		commandList->DrawInstanced(_drawables[dIdx].Instance.VertexCount, 1, 0, 0);
	}
}

void DX::RenderState::CreateRootSignature(const RenderStateDesc& desc)
{
	// TODO: This definitely needs more flexibility
	D3D12_ROOT_PARAMETER rootParams[1];
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].Descriptor.ShaderRegister = 0;
	rootParams[0].Descriptor.RegisterSpace = 0;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;


	CD3DX12_ROOT_SIGNATURE_DESC rootDesc{};
	rootDesc.Init(
		desc.HasConstantBuffer ? 1 : 0, 
		desc.HasConstantBuffer ? rootParams : nullptr, 
		0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signatureBlob;
	ComPtr<ID3DBlob> errorBlob;

	D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	_framework->GetDevice()->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&_rootSignature));
}

void DX::RenderState::GenerateBufferViewAndResource(const Drawable& forDrawable, DrawableData& targetData)
{
	auto device = _framework->GetDevice();
	auto commandList = _renderer->ResetAndGetCommandList();

	D3D12_RESOURCE_DESC vBufDesc{};
	vBufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vBufDesc.Alignment = 0;
	vBufDesc.Width = forDrawable.ByteSize();
	vBufDesc.Height = 1;
	vBufDesc.DepthOrArraySize = 1;
	vBufDesc.MipLevels = 1;
	vBufDesc.Format = DXGI_FORMAT_UNKNOWN;
	vBufDesc.SampleDesc.Count = 1;
	vBufDesc.SampleDesc.Quality = 0;
	vBufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	vBufDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	// 1. Creating the GPU Vertex Buffer
	{
		D3D12_HEAP_PROPERTIES heapProps{};
		heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &vBufDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&targetData.VertexBufferResource));
		targetData.VertexBufferResource.Get()->SetName(L"Vertex Buffer Drawable");
	}

	ComPtr<ID3D12Resource> uploadBuffer;

	// 2. Creating the Upload Buffer
	{
		D3D12_HEAP_PROPERTIES heapProps{};
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &vBufDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer));
	}

	// 3. Copying data to upload buffer
	void* gpuMapVertexData = nullptr;
	D3D12_RANGE nilRange{ 0,0 };
	uploadBuffer.Get()->Map(0, &nilRange, &gpuMapVertexData);
	Util::MemCopy(gpuMapVertexData, forDrawable.VertexDataBuffer, forDrawable.ByteSize());
	uploadBuffer.Get()->Unmap(0, nullptr);

	// 4. Copy data from upload buffer to gpu resource
	commandList->CopyBufferRegion(targetData.VertexBufferResource.Get(), 0, uploadBuffer.Get(), 0, forDrawable.ByteSize());

	// Transition the vertex buffer to a constant state (we're done writing to it)
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = targetData.VertexBufferResource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &barrier);

	// Now push this back and wait for gpu upload
	_renderer->RunCommandListAndAwaitGPUCompletion();

	// 5. Create the view object
	targetData.VertBufferView.BufferLocation = targetData.VertexBufferResource.Get()->GetGPUVirtualAddress();
	targetData.VertBufferView.StrideInBytes = forDrawable.VertexSize;
	targetData.VertBufferView.SizeInBytes = forDrawable.ByteSize();
}
