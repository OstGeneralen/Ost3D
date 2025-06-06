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
			bool CreateCommandList();
			bool CreateGPUSyncObjects();
			IDXGIAdapter1* RequestHardwareAdapter();


		private:
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

			ComPtr<ID3D12DescriptorHeap> _rtvHeap;
			unsigned _rtvHeapSize = 0;

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