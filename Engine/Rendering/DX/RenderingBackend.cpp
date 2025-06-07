#include "RenderingBackend.h"
#include "ShaderCompiler.h"
#include <Engine/Utility/Logging/Logging.h>

STATIC_LOG(DXRendering);

using namespace ost::dx;

ost::dx::RenderingBackend::RenderingBackend()
{
}

bool RenderingBackend::InitializeForWindow(Window& targetWindow)
{
	bool success = true;

	if (success) success &= CreateFactoryAndDevice();
	if (success) success &= CreateCommandQueue();
	if (success) success &= CreateSwapChain(targetWindow);
	if (success) success &= CreateRenderTargetViewHeap();
	if (success) success &= CreateShaderResourceViewHeap(_srvActiveHeap, true, 128);
	if (success) success &= CreateShaderResourceViewHeap(_srvGUIHeap, true, 128);
	if (success) success &= CreateCommandList();
	if (success) success &= CreateGPUSyncObjects();

	_windowResizeEventListener = targetWindow.Event_Resize.Listen([this](const auto& d) { ProcessWindowResize(d); });

	return success;
}

void RenderingBackend::ExecuteQueuedCommandsAndAwaitGPU()
{
	_commandList.Get()->Close();

	ID3D12CommandList* cmdLists[] = { _commandList.Get() };
	_commandQueue.Get()->ExecuteCommandLists(1, cmdLists);

	unsigned long long pushFenceValue = _currentFenceValue;
	_commandQueue.Get()->Signal(_commandFence.Get(), pushFenceValue);
	_currentFenceValue++;
	if (_commandFence.Get()->GetCompletedValue() < pushFenceValue)
	{
		_commandFence.Get()->SetEventOnCompletion(pushFenceValue, _bufferFenceEvent);
		WaitForSingleObjectEx(_bufferFenceEvent, INFINITE, FALSE);
	}
}

ost::RenderState ost::dx::RenderingBackend::CreateRenderState(const RenderStateDesc& desc)
{
	// Create the root signature for this state
	// Todo: If the state wants any constant buffers we are going to set these up here
	CD3DX12_ROOT_SIGNATURE_DESC rootDesc{};
	rootDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signatureBlob;
	ComPtr<ID3DBlob> errorBlob;


	if (FAILED(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob)))
	{
		DXRendering.LOG_ERROR("Failed to serialize root signature: {}", errorBlob->GetBufferPointer());
		return {};
	}

	ID3D12RootSignature* rootSignaturePtr;
	if (FAILED(_device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignaturePtr))))
	{
		DXRendering.LOG_ERROR("Failed to create root signature");
		return {};
	}

	// Now we can go ahead and create our actual pipeline state

	// Compile shaders
	ShaderCompiler compiler;
	CompiledShader vertexShader;
	CompiledShader pixelShader;

	{
		ShaderCompileInfo vsCompileStatus = desc.CompileVertexShader(compiler);
		if (vsCompileStatus.Status == false)
		{
			DXRendering.LOG_ERROR("Failed to compile vertex shader: '{}'", vsCompileStatus.ErrorMessage);
			return {};
		}
		vertexShader = vsCompileStatus.Shader;
	}

	{
		ShaderCompileInfo psCompileStatus = desc.CompilePixelShader(compiler);
		if (psCompileStatus.Status == false)
		{
			DXRendering.LOG_ERROR("Failed to compile pixel shader: '{}'", psCompileStatus.ErrorMessage);
			return {};
		}
		pixelShader = psCompileStatus.Shader;
	}

	// Create state
	D3D12_INPUT_ELEMENT_DESC inElemDescs[16];

	void* inElemDescsVoidPtr = inElemDescs; // Thank you C++ for trying to protect me but also screw you for trying to protect me :)
	desc.PopulateInputElementsDesc(&inElemDescsVoidPtr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC stateDesc{};
	stateDesc.InputLayout.NumElements = desc.GetInputElementNum();
	stateDesc.InputLayout.pInputElementDescs = inElemDescs;
	stateDesc.pRootSignature = rootSignaturePtr;
	stateDesc.VS = CD3DX12_SHADER_BYTECODE{ vertexShader.CompiledData, vertexShader.CompiledDataSize };
	stateDesc.PS = CD3DX12_SHADER_BYTECODE{ pixelShader.CompiledData, pixelShader.CompiledDataSize };

	stateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	stateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	stateDesc.DepthStencilState.DepthEnable = desc.DepthEnabled;
	stateDesc.DepthStencilState.StencilEnable = desc.StencilEnabled;

	stateDesc.SampleMask = UINT_MAX;
	stateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	stateDesc.NumRenderTargets = 1;
	stateDesc.RTVFormats[0] = _rtvFormat;
	stateDesc.SampleDesc.Count = 1;
	stateDesc.SampleDesc.Quality = 0;
	
	ID3D12PipelineState* psoPtr;
	if (FAILED(_device->CreateGraphicsPipelineState(&stateDesc, IID_PPV_ARGS(&psoPtr))))
	{
		DXRendering.LOG_ERROR("Failed to create pipeline state");
		return {};
	}

	RenderState createdState;
	createdState._psoPtr = psoPtr;
	createdState._rootSigPtr = rootSignaturePtr;
	return createdState;
}

void ost::dx::RenderingBackend::ReleaseRenderState(RenderState& state)
{
	ID3D12RootSignature* rootSigPtr = (ID3D12RootSignature*)(state._rootSigPtr);
	ID3D12PipelineState* psoPtr = (ID3D12PipelineState*)(state._psoPtr);

	psoPtr->Release();
	rootSigPtr->Release();
	state._psoPtr = nullptr;
	state._rootSigPtr = nullptr;
}

void RenderingBackend::Release()
{
	_windowResizeEventListener.Reset();

	AwaitGPU();

	_commandFence.Reset();

	for (int i = 0; i < BufferingNum; ++i)
	{
		_commandAllocators[i].Reset();
		_renderTargets[i].Reset();
	}

	_commandList.Reset();
	_commandQueue.Reset();

	_srvActiveHeap.Reset();
	_srvStagingHeap.Reset();
	_rtvHeap.Reset();

	_swapchain.Reset();
	_device.Reset();
	_factory.Reset();
}

// ------------------------------------------------------------
// Render loop
// ------------------------------------------------------------

void RenderingBackend::BeginFrame(RGBAColor_f32 clearColor)
{
	unsigned long long currentFrameResourceIndex = _swapchain->GetCurrentBackBufferIndex();

	// If the previous instance of the frame buffer we're now trying to enter has not finished, wait for the GPU to be done
	// Obviously not ideal but ain't no nice way to get around this
	// We could triple buffer but that only lowers the chance of this happening

	unsigned long long completedValue = _commandFence->GetCompletedValue();

	if (_commandFenceValues[currentFrameResourceIndex] != 0 && completedValue < _commandFenceValues[currentFrameResourceIndex])
	{
		_commandFence.Get()->SetEventOnCompletion(_commandFenceValues[currentFrameResourceIndex], _bufferFenceEvent);
		WaitForSingleObjectEx(_bufferFenceEvent, INFINITE, FALSE);
	}


	_commandAllocators[currentFrameResourceIndex].Get()->Reset();
	_commandList.Get()->Reset(_commandAllocators[currentFrameResourceIndex].Get(), nullptr);

	D3D12_VIEWPORT vp{};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = static_cast<FLOAT>(_windowDimensions.X);
	vp.Height = static_cast<FLOAT>(_windowDimensions.Y);
	_commandList.Get()->RSSetViewports(1, &vp);

	D3D12_RECT sr{};
	sr.left = 0;
	sr.right = 0;
	sr.right = _windowDimensions.X;
	sr.bottom = _windowDimensions.Y;
	_commandList.Get()->RSSetScissorRects(1, &sr);

	auto transitionToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		_renderTargets[currentFrameResourceIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	_commandList.Get()->ResourceBarrier(1, &transitionToRenderTarget);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), currentFrameResourceIndex, _rtvHeapSize);
	_commandList.Get()->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	ID3D12DescriptorHeap* dh[]{ _srvActiveHeap.Get() };
	_commandList->SetDescriptorHeaps(1, dh);

	float cc[4];
	clearColor.CopyToFloatBuffer(cc);
	_commandList.Get()->ClearRenderTargetView(rtvHandle, cc, 0, nullptr);

	_bufferIndex = currentFrameResourceIndex;
}

void ost::dx::RenderingBackend::SetActiveRenderState(RenderState& state)
{
	if (!state.Valid()) return;

	_commandList->SetGraphicsRootSignature((ID3D12RootSignature*)( state._rootSigPtr));
	_commandList->SetPipelineState((ID3D12PipelineState*)(state._psoPtr));
}

void RenderingBackend::EndAndPresentFrame()
{
	// Transition current buffer to present state
	auto transitionToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		_renderTargets[_bufferIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	_commandList.Get()->ResourceBarrier(1, &transitionToPresent);

	// Close the command list and transfer commands to GPU
	_commandList.Get()->Close();

	ID3D12CommandList* cmdListPP[] = { _commandList.Get() };
	_commandQueue.Get()->ExecuteCommandLists(1, cmdListPP);

	// Present the frame and signal the fences
	_swapchain.Get()->Present(1, 0);

	_commandQueue.Get()->Signal(_commandFence.Get(), _currentFenceValue);
	_commandFenceValues[_bufferIndex] = _currentFenceValue;

	_currentFenceValue++;
}

// ------------------------------------------------------------
// DX Creation Methods
// ------------------------------------------------------------

bool RenderingBackend::CreateFactoryAndDevice()
{

	unsigned factoryFlags = 0;
#if _DEBUG
	ID3D12Debug* debugPtr;
	if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugPtr))))
	{
		return false;
	}
	else
	{
		debugPtr->EnableDebugLayer();
		factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	if (FAILED(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&_factory))))
	{
		return false;
	}
	if (FAILED(D3D12CreateDevice(RequestHardwareAdapter(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device))))
	{
		return false;
	}
	return true;
}

bool RenderingBackend::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	if (FAILED((_device.Get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_commandQueue)))))
	{
		return false;
	}
	return true;
}

bool RenderingBackend::CreateSwapChain(const Window& targetWindow)
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.BufferCount = BufferingNum;
	desc.Width = targetWindow.GetDimensions().X;
	desc.Height = targetWindow.GetDimensions().Y;
	desc.Format = _rtvFormat;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;

	IDXGISwapChain1* ss1;
	if (FAILED(_factory.Get()->CreateSwapChainForHwnd(
		_commandQueue.Get(),
		targetWindow.GetHWND(),
		&desc,
		nullptr, nullptr,
		&ss1)))
	{
		return false;
	}

	if (FAILED(ss1->QueryInterface(IID_PPV_ARGS(&_swapchain))))
	{
		return false;
	}

	_factory->MakeWindowAssociation(targetWindow.GetHWND(), DXGI_MWA_NO_ALT_ENTER);
	_bufferIndex = _swapchain->GetCurrentBackBufferIndex();

	return true;
}

bool RenderingBackend::CreateRenderTargetViewHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = BufferingNum;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	if (FAILED(_device.Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_rtvHeap))))
	{
		return false;
	}

	_rtvHeapSize = _device.Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (unsigned i = 0; i < BufferingNum; ++i)
	{
		if (FAILED(_swapchain->GetBuffer(i, IID_PPV_ARGS(&(_renderTargets[i])))))
		{
			return false;
		}

		_device.Get()->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, _rtvHeapSize);
		if (FAILED(
			_device.Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&(_commandAllocators[i])))
		))
		{
			return false;
		}
	}

	return true;
}

bool ost::dx::RenderingBackend::CreateShaderResourceViewHeap(ComPtr<ID3D12DescriptorHeap>& heapPtr, bool shaderVisible, unsigned numDescs)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = numDescs;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (shaderVisible)
	{
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	}

	desc.NodeMask = 0;

	if (FAILED(_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heapPtr))))
	{
		return false;
	}

	if (shaderVisible)
	{
		heapPtr->SetName(L"Active SRV Heap");
	}
	else
	{
		heapPtr->SetName(L"Staging SRV Heap");
	}
	_srvHeapSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

bool RenderingBackend::CreateCommandList()
{
	if (FAILED(
		_device.Get()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			_commandAllocators[_bufferIndex].Get(),
			nullptr, // <--- No pipeline state at this point. This needs to be set before any actual rendering.
			IID_PPV_ARGS(&_commandList))
	))
	{
		return false;
	}

	_commandList->Close();
	return true;
}

bool RenderingBackend::CreateGPUSyncObjects()
{
	if (FAILED(_device.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_commandFence))))
	{
		return false;
	}
	_currentFenceValue = 1;
	_bufferFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	return true;
}

IDXGIAdapter1* RenderingBackend::RequestHardwareAdapter()
{
	ComPtr<IDXGIAdapter1> adapter;
	ComPtr<IDXGIFactory6> factory6;

	if (SUCCEEDED(_factory->QueryInterface(IID_PPV_ARGS(&factory6))))
	{
		for (unsigned adapterIdx = 0;
			SUCCEEDED(factory6->EnumAdapterByGpuPreference(adapterIdx, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));
			++adapterIdx)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue; // Skip if software rendering

			// Just checking so that the adapter supports DX12 here (hence passing nullptr as the device arg)
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
			{
				return adapter.Detach();
			}
		}
	}

	return nullptr;
}

void ost::dx::RenderingBackend::AwaitGPU()
{
	unsigned long long awaitValue = _currentFenceValue;
	_commandQueue->Signal(_commandFence.Get(), awaitValue);

	_currentFenceValue++;

	_commandFence->SetEventOnCompletion(awaitValue, _bufferFenceEvent);
	WaitForSingleObjectEx(_bufferFenceEvent, INFINITE, FALSE);
}


#include <Engine/Editor/ImGui/imgui.h>
void ost::dx::RenderingBackend::DisplayGUI()
{
	{ // Swapchain info
		DXGI_SWAP_CHAIN_DESC swapchainDesc;
		_swapchain->GetDesc(&swapchainDesc);

		ImGui::Text("SWAPCHAIN");
		ImGui::Text("Buffer count: %i", swapchainDesc.BufferCount);
		ImGui::Text("Buffer dimensions: %ix%i", swapchainDesc.BufferDesc.Width, swapchainDesc.BufferDesc.Height);
	}
}

void ost::dx::RenderingBackend::ProcessWindowResize(const Dimensions& newSize)
{
	AwaitGPU();

	_windowDimensions = newSize;
	// Now we can resize
	for (unsigned i = 0; i < BufferingNum; ++i)
	{
		_renderTargets[i].Reset();
	}

	_swapchain->ResizeBuffers(BufferingNum, _windowDimensions.X, _windowDimensions.Y, _rtvFormat, 0);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (unsigned i = 0; i < BufferingNum; ++i)
	{
		if (FAILED(_swapchain->GetBuffer(i, IID_PPV_ARGS(&(_renderTargets[i])))))
		{
		}

		_device.Get()->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, _rtvHeapSize);
	}
}

