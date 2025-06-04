#include "Renderer.h"
#include "Framework.h"

#include "../Window/Window.h"

#define EXTRACT_FRAMEWORK_FACTORY *(_framework->GetFactory()) 
#define EXTRACT_FRAMEWORK_DEVICE *(_framework->GetDevice()) 

using namespace DX;

bool DX::Renderer::Initialize(Framework* framework, const Window& targetWindow)
{
	_windowSize = targetWindow.GetDimensions();
	_framework = framework;

	bool success = true;

	if (success) success &= CreateCommandQueue();
	if (success) success &= CreateSwapChain(targetWindow);
	if (success) success &= CreateRenderTargetViewHeap();
	if (success) success &= CreateCommandList();
	if (success) success &= CreateGPUSyncObjects();

	return success;
}

void Renderer::BeginRenderFrame(RGBAColor_u8 clearColor)
{
	_commandAllocators[_frameIndex].Get()->Reset();
	_commandList.Get()->Reset(_commandAllocators[_frameIndex].Get(), nullptr);

	D3D12_VIEWPORT vp{};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = _windowSize.X;
	vp.Height = _windowSize.Y;
	_commandList->RSSetViewports(1, &vp);

	D3D12_RECT sr{};
	sr.left = 0;
	sr.right = 0;
	sr.right = _windowSize.X;
	sr.bottom = _windowSize.Y;

	auto transitionToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		_renderTargets[_frameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	_commandList->ResourceBarrier(1, &transitionToRenderTarget);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart(), _frameIndex, _rtvHeapSize);
	_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	float cc[4];
	clearColor.ToRGBA_f32().CopyToFloatBuffer(cc);
	_commandList->ClearRenderTargetView(rtvHandle, cc, 0, nullptr);
}

void DX::Renderer::EndRenderFrame()
{
	auto transitionToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		_renderTargets[_frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);

	_commandList->ResourceBarrier(1, &transitionToPresent);
	_commandList->Close();
}

void DX::Renderer::PresentFrame()
{
	ID3D12CommandList* cmdListPP[] = { _commandList.Get() };
	_commandQueue->ExecuteCommandLists(1, cmdListPP);
	_swapChain->Present(1, 0);

	const auto currentFenceValue = _fenceValues[_frameIndex];
	_commandQueue->Signal(_commandFence.Get(), currentFenceValue);
	
	_frameIndex = _swapChain->GetCurrentBackBufferIndex();

	if (_commandFence->GetCompletedValue() < _fenceValues[_frameIndex])
	{
		// We have no ready buffer to render to so await the GPU to finish with it
		_commandFence->SetEventOnCompletion(_fenceValues[_frameIndex], _fenceEvent);
		WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);
	}
	_fenceValues[_frameIndex] = _fenceValues[_frameIndex] + 1;
}

bool DX::Renderer::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	if (FAILED((_framework->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_commandQueue)))))
	{
		return false;
	}
	return true;
}

bool DX::Renderer::CreateSwapChain(const Window& targetWindow)
{
	auto& factory = EXTRACT_FRAMEWORK_FACTORY;
	auto& device = EXTRACT_FRAMEWORK_DEVICE;

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.BufferCount = FrameCount;
	desc.Width = targetWindow.GetDimensions().X;
	desc.Height = targetWindow.GetDimensions().Y;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;

	IDXGISwapChain1* ss1;
	if (FAILED(factory.CreateSwapChainForHwnd(
		_commandQueue.Get(),
		targetWindow.GetHWND(),
		&desc,
		nullptr, nullptr,
		&ss1)))
	{
		return false;
	}

	if (FAILED(ss1->QueryInterface(IID_PPV_ARGS(&_swapChain))))
	{
		return false;
	}

	factory.MakeWindowAssociation(targetWindow.GetHWND(), DXGI_MWA_NO_ALT_ENTER);
	_frameIndex = _swapChain->GetCurrentBackBufferIndex();

	return true;
}

bool DX::Renderer::CreateRenderTargetViewHeap()
{
	auto& factory = EXTRACT_FRAMEWORK_FACTORY;
	auto& device = EXTRACT_FRAMEWORK_DEVICE;

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = FrameCount;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	if (FAILED(
		device.CreateDescriptorHeap( &desc, IID_PPV_ARGS(&_rtvHeap))
	))
	{
		return false;
	}

	_rtvHeapSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (unsigned i = 0; i < FrameCount; ++i)
	{
		if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(&(_renderTargets[i])))))
		{
			return false;
		}

		device.CreateRenderTargetView(_renderTargets[i].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, _rtvHeapSize);
		if (FAILED(
			device.CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&(_commandAllocators[i])))
		))
		{
			return false;
		}
	}

	return true;
}

bool DX::Renderer::CreateCommandList()
{
	auto& device = EXTRACT_FRAMEWORK_DEVICE;

	if (FAILED(
		device.CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			_commandAllocators[_frameIndex].Get(),
			nullptr, // <--- No pipeline state at this point. This needs to be set before any actual rendering.
			IID_PPV_ARGS(&_commandList))
	))
	{
		return false;
	}

	_commandList->Close();
	return true;
}

bool DX::Renderer::CreateGPUSyncObjects()
{
	auto& device = EXTRACT_FRAMEWORK_DEVICE;

	if (FAILED(
		device.CreateFence(_fenceValues[_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_commandFence))
	))
	{
		return false;
	}

	_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	return true;
}



