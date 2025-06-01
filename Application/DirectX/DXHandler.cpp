#include "DXHandler.h"
#include <d3dx12.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include "../Window/Window.h"

#define RELEASE_AND_NULL(ptr) if(ptr) { ptr->Release(); ptr = nullptr; }

namespace
{
	IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory1* factory)
	{
		IDXGIAdapter1* adapter;
		IDXGIFactory6* factory6;

		if (factory->QueryInterface(IID_PPV_ARGS(&factory6)) == S_OK)
		{
			for (UINT adapterIdx = 0; 
				SUCCEEDED(factory6->EnumAdapterByGpuPreference(
					adapterIdx,
					DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
					IID_PPV_ARGS(&adapter)));
				++adapterIdx)
			{
				DXGI_ADAPTER_DESC1 adapterDesc;
				adapter->GetDesc1(&adapterDesc);

				if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// Skip (we don't want software rendering lmao)
					continue;
				}

				if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
				{
					return adapter;
				}
			}
		}
		return nullptr;
	}
}

DXHandler::DXHandler()
	: _debugLayer{nullptr}
	, _dxFactory{ nullptr }
	, _dxDevice{nullptr}
	, _commandQueue{nullptr}
	, _swapChain{nullptr}
	, _rtvHeap{ nullptr }
	, _rtvHeapSize{0u}
	, _renderTargets{nullptr, nullptr}
	, _cmdAlloc{nullptr}
	, _cmdList{nullptr}
	, _renderWidth{0u}
	, _renderHeight{0u}
	, _frameIdx{0u}
{
}

DXHandler::~DXHandler()
{
	Release();
}

DXHandlerInitErrorFlag DXHandler::Initialize(const Window& window)
{
	UINT dxFactoryFlags = 0u;

	_renderWidth = window.GetWidth();
	_renderHeight = window.GetHeight();

#if DXDEBUG_ENABLED
	// Debug layer setup
	if (D3D12GetDebugInterface(IID_PPV_ARGS(&_debugLayer)) == S_OK)
	{
		_debugLayer->EnableDebugLayer();
	}
	else
	{
		return DXHandlerErrors::DebugLayerFailure;
	}
	dxFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	// Factory creation
	if (FAILED(CreateDXGIFactory2(dxFactoryFlags, IID_PPV_ARGS(&_dxFactory))))
	{
		return DXHandlerErrors::FactoryFailure;
	}

	// Device creation
	IDXGIAdapter1* adapter = ::GetHardwareAdapter(_dxFactory);
	if (FAILED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_dxDevice))))
	{
		return DXHandlerErrors::DeviceFailure;
	}

	// Command queue creation
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	if (FAILED(_dxDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&_commandQueue))))
	{
		return DXHandlerErrors::CommandQueueFailure;
	}

	// Swap Chain Creation
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.BufferCount = s_bufferCount; // Double buffer rendering
	swapChainDesc.Width = _renderWidth;
	swapChainDesc.Height = _renderHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1; // This might have to be changed if implementing MSAA etc.
	
	// NOTE: This conversion seems dangerous. Do we ever populate the SwapChain3 parts of this type?
	IDXGISwapChain1* ss1;
	if (FAILED(_dxFactory->CreateSwapChainForHwnd(_commandQueue, window.GetHWND(), &swapChainDesc, nullptr, nullptr, &ss1)))
	{
		return DXHandlerErrors::SwapChainFailure;
	}
	if (FAILED(ss1->QueryInterface(IID_PPV_ARGS(&_swapChain))))
	{
		// Lmao wtf?
		return DXHandlerErrors::SwapChainConvertFailure;
	}

	_dxFactory->MakeWindowAssociation(window.GetHWND(), DXGI_MWA_NO_ALT_ENTER);
	
	_frameIdx = _swapChain->GetCurrentBackBufferIndex();

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.NumDescriptors = s_bufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	
	if (FAILED(_dxDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap))))
	{
		return DXHandlerErrors::DescriptorHeapFailure;
	}

	_rtvHeapSize = _dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (unsigned int i = 0; i < s_bufferCount; ++i)
	{
		if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(&(_renderTargets[i])))))
		{
			return DXHandlerErrors::BackBufferFailure;
		}

		_dxDevice->CreateRenderTargetView(_renderTargets[i], nullptr, rtvHandle);
		rtvHandle.Offset(1, _rtvHeapSize);
	}
	
	if (FAILED(_dxDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc))))
	{
		return DXHandlerErrors::CommandAllocatorFailure;
	}

	return DXHandlerErrors::Success;
}

void DXHandler::Release()
{
	RELEASE_AND_NULL(_cmdList);
	RELEASE_AND_NULL(_cmdAlloc);
	for (int i = 0; i < s_bufferCount; ++i)
	{
		RELEASE_AND_NULL(_renderTargets[i]);
	}
	RELEASE_AND_NULL(_rtvHeap);
	RELEASE_AND_NULL(_swapChain);
	RELEASE_AND_NULL(_commandQueue);
	RELEASE_AND_NULL(_dxDevice);
	RELEASE_AND_NULL(_dxFactory);
	_renderWidth = 0u;
	_renderHeight = 0u;
	_frameIdx = 0u;
}