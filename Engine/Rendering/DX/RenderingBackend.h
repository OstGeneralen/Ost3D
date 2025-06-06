#pragma once
#include "DXCore.h"
#include <Engine/Rendering/Window/Window.h>
#include <Engine/Rendering/Color/Color.h>

namespace ost
{
	namespace dx
	{
		constexpr static unsigned BufferingNum = 2;

		class RenderingBackend
		{
		public:
			bool InitializeForWindow(const Window& targetWindow);
			void Release();
			void ExecuteQueuedCommandsAndAwaitGPU();

		public:
			// Render loop functions
			void BeginFrame(RGBAColor_f32 clearColor);
			void EndAndPresentFrame();

		private:
			bool CreateFactoryAndDevice();
			bool CreateCommandQueue();
			bool CreateSwapChain(const Window& targetWindow);
			bool CreateRenderTargetViewHeap();
			bool CreateShaderResourceViewHeap(ComPtr<ID3D12DescriptorHeap>& heapPtr, bool shaderVisible, unsigned numDescs);
			bool CreateCommandList();
			bool CreateGPUSyncObjects();
			IDXGIAdapter1* RequestHardwareAdapter();

			void AwaitGPU();

		public: // External access
			ID3D12Device* GetDevice() const { return _device.Get(); }
			ID3D12CommandQueue* GetCommandQueue() const { return _commandQueue.Get(); }
			DXGI_FORMAT GetRenderTargetFormat() const { return _rtvFormat; }
			DXGI_FORMAT GetDepthStencilFormat() const { return _dsvFormat; }

			ID3D12DescriptorHeap* GetSRVHeap(bool getActive) const { return getActive ? _srvActiveHeap.Get() : _srvStagingHeap.Get(); }
			ID3D12DescriptorHeap* GetSRVHeapGUI() const { return _srvGUIHeap.Get(); }
			ID3D12GraphicsCommandList* GetCommandList() const { return _commandList.Get(); }

		private:
			// Setup info
			const DXGI_FORMAT _rtvFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
			const DXGI_FORMAT _dsvFormat{ DXGI_FORMAT_UNKNOWN };

			// DX Core
			ComPtr<IDXGIFactory4> _factory;
			ComPtr<ID3D12Device> _device;

			// Rendering Objects
			ComPtr<IDXGISwapChain3> _swapchain;
			ComPtr<ID3D12CommandQueue> _commandQueue;
			ComPtr<ID3D12GraphicsCommandList> _commandList;

			// Frame Buffer Resources
			ComPtr<ID3D12Resource> _renderTargets[BufferingNum];
			ComPtr<ID3D12CommandAllocator> _commandAllocators[BufferingNum];

			// Render target view heap
			ComPtr<ID3D12DescriptorHeap> _rtvHeap;
			unsigned _rtvHeapSize = 0;

			// Shader resource view heap
			ComPtr<ID3D12DescriptorHeap> _srvGUIHeap;	// Shader visible, used during draw calls
			ComPtr<ID3D12DescriptorHeap> _srvActiveHeap;	// Shader visible, used during draw calls
			ComPtr<ID3D12DescriptorHeap> _srvStagingHeap;	// Staging of data, not visible to shaders, copy content into active on render
			unsigned _srvHeapSize = 0;

			// Sync objects
			unsigned _bufferIndex;
			HANDLE _bufferFenceEvent;
			ComPtr<ID3D12Fence> _commandFence;
			unsigned long long _commandFenceValues[BufferingNum] = { 0,0 };
			unsigned long long _currentFenceValue = 0;

			// Stored data
			Dimensions _windowDimensions;
		};
	}
}