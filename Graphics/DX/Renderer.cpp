#include "Renderer.h"
#include "Framework.h"
#include "RenderState.h"

#include "../Window/Window.h"

#include <pix3.h>

#define EXTRACT_FRAMEWORK_FACTORY *(_framework->GetFactory()) 
#define EXTRACT_FRAMEWORK_DEVICE *(_framework->GetDevice()) 

#define SCOPED_PIX_EVENT( name, code ) PIXBeginEvent(0xFF0000FF, name); code PIXEndEvent();

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

	unsigned long long currentFrameResourceIndex = _swapChain->GetCurrentBackBufferIndex();

	if (_fenceValues[currentFrameResourceIndex] != 0 && _commandFence->GetCompletedValue() < _fenceValues[currentFrameResourceIndex])
	{
		_commandFence.Get()->SetEventOnCompletion(_fenceValues[currentFrameResourceIndex], _fenceEvent);
		WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);
	}

	_commandAllocators[currentFrameResourceIndex].Get()->Reset();
	_commandList.Get()->Reset(_commandAllocators[currentFrameResourceIndex].Get(), nullptr);

	D3D12_VIEWPORT vp{};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = static_cast<FLOAT>(_windowSize.X);
	vp.Height = static_cast<FLOAT>(_windowSize.Y);
	_commandList.Get()->RSSetViewports(1, &vp);

	D3D12_RECT sr{};
	sr.left = 0;
	sr.right = 0;
	sr.right = _windowSize.X;
	sr.bottom = _windowSize.Y;
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
	clearColor.ToRGBA_f32().CopyToFloatBuffer(cc);
	_commandList.Get()->ClearRenderTargetView(rtvHandle, cc, 0, nullptr);

	_frameIndex = currentFrameResourceIndex;
}

void DX::Renderer::EndRenderFrame()
{
	auto transitionToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		_renderTargets[_frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);

	_commandList.Get()->ResourceBarrier(1, &transitionToPresent);
	_commandList.Get()->Close();

	ID3D12CommandList* cmdListPP[] = { _commandList.Get() };
	_commandQueue.Get()->ExecuteCommandLists(1, cmdListPP);
}

void DX::Renderer::PresentFrame()
{
	_swapChain.Get()->Present(1, 0);

	//const auto currentFenceValue = _fenceValues[_frameIndex];
	_commandQueue.Get()->Signal(_commandFence.Get(), _currentFenceValue);
	_fenceValues[_frameIndex] = _currentFenceValue;

	//_frameIndex = _swapChain.Get()->GetCurrentBackBufferIndex();


	//if (_fenceValues[_frameIndex] != 0 && _commandFence.Get()->GetCompletedValue() < _fenceValues[_frameIndex])
	//{
	//	// We have no ready buffer to render to so await the GPU to finish with it
	//	_commandFence.Get()->SetEventOnCompletion(_fenceValues[_frameIndex], _fenceEvent);
	//	WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);
	//}
	_currentFenceValue++;
	//_fenceValues[_frameIndex] = currentFenceValue + 1;
}

void DX::Renderer::ExecuteRenderState(const RenderStateHandle& handle)
{
	_renderStates[handle.GetStateID()].RendererDraw(_commandList.Get());
}

RenderStateHandle Renderer::CreateRenderState(const RenderStateDesc& desc)
{
	RenderState& nRenderState = _renderStates.Emplace(_framework, desc);
	nRenderState.Initialize(desc);

	return RenderStateHandle{ nRenderState, _renderStates.Size() - 1 };
}

ID3D12GraphicsCommandList* DX::Renderer::ResetAndGetCommandList()
{
	_commandAllocators[_frameIndex].Get()->Reset();
	_commandList.Get()->Reset(_commandAllocators[_frameIndex].Get(), nullptr);
	return _commandList.Get();
}

void DX::Renderer::RunCommandListAndAwaitGPUCompletion()
{
	_commandList.Get()->Close();

	ID3D12CommandList* cmdLists[] = { _commandList.Get() };
	_commandQueue.Get()->ExecuteCommandLists(1, cmdLists);

	unsigned long long pushFenceValue = _currentFenceValue;
	_commandQueue.Get()->Signal(_commandFence.Get(), pushFenceValue);
	_currentFenceValue++;
	if (_commandFence.Get()->GetCompletedValue() < pushFenceValue)
	{
		_commandFence.Get()->SetEventOnCompletion(pushFenceValue, _fenceEvent);
		WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);
	}
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
		device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_rtvHeap))
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
		device.CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_commandFence))
	))
	{
		return false;
	}
	_currentFenceValue = 1;
	_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	return true;
}



