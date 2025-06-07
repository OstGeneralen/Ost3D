#include "GUIHandler.h"
#include <Engine/Rendering/Window/Window.h>
#include <Engine/Rendering/DX/RenderingBackend.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"

struct GUISRVHeapAllocator
{
	ID3D12DescriptorHeap*		Heap{ nullptr };
	D3D12_DESCRIPTOR_HEAP_TYPE	HeapType{};
	D3D12_CPU_DESCRIPTOR_HANDLE HeapStartCpu;
	D3D12_GPU_DESCRIPTOR_HANDLE HeapStartGpu;
	UINT                        HeapHandleIncrement;
	ImVector<int>               FreeIndices;

	void Create(ID3D12Device* device, ID3D12DescriptorHeap* heap)
	{
		Heap = heap;
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = Heap->GetDesc();
		HeapType = heapDesc.Type;
		HeapStartCpu = Heap->GetCPUDescriptorHandleForHeapStart();
		HeapStartGpu = Heap->GetGPUDescriptorHandleForHeapStart();
		HeapHandleIncrement = device->GetDescriptorHandleIncrementSize(HeapType);
		FreeIndices.reserve((int)heapDesc.NumDescriptors);
		for (int n = heapDesc.NumDescriptors; n > 0; --n)
		{
			FreeIndices.push_back(n - 1);
		}
	}

	void Destroy()
	{
		Heap = nullptr;
		FreeIndices.clear();
	}

	void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle)
	{
		IM_ASSERT(FreeIndices.Size > 0);
		int idx = FreeIndices.back();
		FreeIndices.pop_back();
		out_cpu_desc_handle->ptr = HeapStartCpu.ptr + (idx * HeapHandleIncrement);
		out_gpu_desc_handle->ptr = HeapStartGpu.ptr + (idx * HeapHandleIncrement);
	}

	void Free(D3D12_CPU_DESCRIPTOR_HANDLE out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE out_gpu_desc_handle)
	{
		int cpu_idx = (int)((out_cpu_desc_handle.ptr - HeapStartCpu.ptr) / HeapHandleIncrement);
		int gpu_idx = (int)((out_gpu_desc_handle.ptr - HeapStartGpu.ptr) / HeapHandleIncrement);
		IM_ASSERT(cpu_idx == gpu_idx);
		FreeIndices.push_back(cpu_idx);
	}
	
};

GUISRVHeapAllocator guiSrvAllocator;


void ost::gui::GUIHandler::Init(Window& appWindow, const dx::RenderingBackend& renderBackend)
{
	_windowResizeListener = appWindow.Event_Resize.Listen([this](const auto& d) {ProcessWindowResize(d); });

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.DisplaySize = { (float)appWindow.GetWidth(), (float)appWindow.GetHeight() };
	_windowDimensions = appWindow.GetDimensions();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(appWindow.GetHWND());
	ImGui_ImplWin32_EnableDpiAwareness();

	ImGui_ImplDX12_InitInfo initInfo{};
	initInfo.Device = renderBackend.GetDevice();
	initInfo.CommandQueue = renderBackend.GetCommandQueue();
	initInfo.NumFramesInFlight = 2;
	initInfo.RTVFormat = renderBackend.GetRenderTargetFormat();
	initInfo.DSVFormat = renderBackend.GetDepthStencilFormat();

	ID3D12DescriptorHeap* srvHeapPtr = renderBackend.GetSRVHeapGUI();
	guiSrvAllocator.Create(renderBackend.GetDevice(), srvHeapPtr);
	initInfo.SrvDescriptorHeap = srvHeapPtr;
	initInfo.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle) { return guiSrvAllocator.Alloc(out_cpu_handle, out_gpu_handle); };
	initInfo.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle) { return guiSrvAllocator.Free(cpu_handle, gpu_handle); };

	ImGui_ImplDX12_Init(&initInfo);
}

void ost::gui::GUIHandler::Uninit()
{
	_windowResizeListener.Reset();

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ost::gui::GUIHandler::BeginGuiFrame()
{
	ImGui::GetIO().DisplaySize = { static_cast<float>(_windowDimensions.X), static_cast<float>(_windowDimensions.Y) };
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ost::gui::GUIHandler::EndGuiFrame(const dx::RenderingBackend& renderBackend)
{
	ID3D12DescriptorHeap* guiDescHeap[]{ renderBackend.GetSRVHeapGUI() };
	renderBackend.GetCommandList()->SetDescriptorHeaps(1, guiDescHeap);
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderBackend.GetCommandList());
}

void ost::gui::GUIHandler::ProcessWindowResize(const Dimensions& newDim)
{
	_windowDimensions = newDim;
}