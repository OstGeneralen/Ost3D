#include "RenderingBackend.h"

using namespace ost::dx;

bool RenderingBackend::InitializeForWindow(const Window& targetWindow)
{
	bool success = true;

	if (success) success &= CreateFactoryAndDevice();
	if (success) success &= CreateCommandQueue();
	if (success) success &= CreateSwapChain(targetWindow);
	if (success) success &= CreateRenderTargetViewHeap();
	if (success) success &= CreateCommandList();
	if (success) success &= CreateGPUSyncObjects();

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

// ------------------------------------------------------------
// Render loop
// ------------------------------------------------------------

void RenderingBackend::BeginFrame(RGBAColor_f32 clearColor)
{
	unsigned long long currentFrameResourceIndex = _swapchain->GetCurrentBackBufferIndex();

	// If the previous instance of the frame buffer we're now trying to enter has not finished, wait for the GPU to be done
	// Obviously not ideal but ain't no nice way to get around this
	// We could triple buffer but that only lowers the chance of this happening
	if (_commandFenceValues[currentFrameResourceIndex] != 0 && _commandFence->GetCompletedValue() < _commandFenceValues[currentFrameResourceIndex])
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

	float cc[4];
	clearColor.CopyToFloatBuffer(cc);
	_commandList.Get()->ClearRenderTargetView(rtvHandle, cc, 0, nullptr);

	_bufferIndex = currentFrameResourceIndex;
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
	if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&_factory))))
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
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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

