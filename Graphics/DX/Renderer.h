#pragma once
#include <Graphics/DX/DXCore.h>
#include <Common/Rendering/Color.h>
#include <Graphics/Rendering/RenderingFramework.h>
#include <Graphics/Rendering/RenderState.h>
#include <Common/Container/DynamicList.h>
#include <Graphics/DX/RenderState.h>

class Window;

namespace DX
{
	class Framework;

	class Renderer : public IRenderer
	{
	public:
		constexpr static unsigned FrameCount = 2u;

	public:
		bool Initialize(Framework* framework, const Window& targetWindow);

		void BeginRenderFrame( RGBAColor_u8 clearColor ) override;
		void EndRenderFrame() override;
		void PresentFrame() override;

		void ExecuteRenderState(const RenderStateHandle& handle) override;
		RenderStateHandle CreateRenderState(const RenderStateDesc& desc) override;

		ID3D12GraphicsCommandList* ResetAndGetCommandList();
		void RunCommandListAndAwaitGPUCompletion();
	private:
		bool CreateCommandQueue();
		bool CreateSwapChain(const Window& targetWindow);
		bool CreateRenderTargetViewHeap();
		bool CreateCommandList();
		bool CreateGPUSyncObjects();

	private:
		ComPtr<IDXGISwapChain3> _swapChain;
		ComPtr<ID3D12RootSignature> _rootSignature;

		// Render target
		ComPtr<ID3D12Resource> _renderTargets[FrameCount];
		ComPtr<ID3D12DescriptorHeap> _rtvHeap;
		unsigned _rtvHeapSize = 0;

		// Commands
		ComPtr<ID3D12CommandQueue> _commandQueue;
		ComPtr<ID3D12GraphicsCommandList> _commandList;
		ComPtr<ID3D12CommandAllocator> _commandAllocators[FrameCount];

		// GPU Sync
		unsigned _frameIndex = 0;
		HANDLE _fenceEvent;
		ComPtr<ID3D12Fence> _commandFence;
		unsigned long long _fenceValues[FrameCount] = {0,0};
		unsigned long long _currentFenceValue = 0;
		
		// States
		TDynamicList<RenderState> _renderStates;

		Framework* _framework;

		Dimensions _windowSize;
	};
}