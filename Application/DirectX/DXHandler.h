#pragma once
#include <Windows.h>

// ------------------------------------------------------------

#define DXDEBUG_ENABLED 1

// ------------------------------------------------------------

struct ID3D12Debug;
struct IDXGIFactory4;
struct ID3D12Device;
struct ID3D12CommandQueue;
struct IDXGISwapChain3;
struct ID3D12DescriptorHeap;
struct ID3D12CommandAllocator;
struct ID3D12CommandList;
struct ID3D12Resource;

class Window;

using DXHandlerInitErrorFlag = unsigned int;
namespace DXHandlerErrors
{
	constexpr static DXHandlerInitErrorFlag Success						= 0;
	constexpr static DXHandlerInitErrorFlag DebugLayerFailure			= 1 << 0;
	constexpr static DXHandlerInitErrorFlag FactoryFailure				= 1 << 1;
	constexpr static DXHandlerInitErrorFlag DeviceFailure				= 1 << 2;
	constexpr static DXHandlerInitErrorFlag CommandQueueFailure			= 1 << 3;
	constexpr static DXHandlerInitErrorFlag SwapChainFailure			= 1 << 4;
	constexpr static DXHandlerInitErrorFlag SwapChainConvertFailure		= 1 << 5;
	constexpr static DXHandlerInitErrorFlag DescriptorHeapFailure		= 1 << 6;
	constexpr static DXHandlerInitErrorFlag BackBufferFailure			= 1 << 7;
	constexpr static DXHandlerInitErrorFlag CommandAllocatorFailure		= 1 << 8;
}

class DXHandler
{
public:
	DXHandler();
	DXHandler(const DXHandler&) = delete;
	DXHandler(DXHandler&&) noexcept = default;
	~DXHandler();
	
public:
	DXHandlerInitErrorFlag Initialize(const Window& window);
	void Release();
private:
	ID3D12Debug* _debugLayer;
	IDXGIFactory4* _dxFactory;
	ID3D12Device* _dxDevice;
	ID3D12CommandQueue* _commandQueue;
	IDXGISwapChain3* _swapChain;
	
	ID3D12DescriptorHeap* _rtvHeap;
	unsigned int _rtvHeapSize;

	constexpr static unsigned int s_bufferCount = 2;
	ID3D12Resource* _renderTargets[s_bufferCount];

	ID3D12CommandAllocator* _cmdAlloc;
	ID3D12CommandList* _cmdList;

	unsigned int _renderWidth;
	unsigned int _renderHeight;

	unsigned int _frameIdx;

};

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------