#include "Framework.h"



#include "../Window/Window.h"

#define RETURN_IF_FAILED( op ) if( FAILED( op ) ) { return false; }
#define MARK_SUCCESS return true;


namespace
{
	IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory1* factory)
	{
		ComPtr<IDXGIAdapter1> adapter;
		ComPtr<IDXGIFactory6> factory6;

		if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
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
}


using namespace DX;



Framework::Framework()
{
}

bool Framework::Initialize(const Window& forWin)
{
	UINT factoryFlags = 0;

#if _DEBUG
	ID3D12Debug* ptr_debugLayer;
	if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&ptr_debugLayer))))
	{
		return false;
	}
	else
	{
		ptr_debugLayer->EnableDebugLayer();
		ptr_debugLayer->QueryInterface(IID_PPV_ARGS(&_debugLayer));
		_debugLayer->SetEnableGPUBasedValidation(TRUE);
	}
	factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	if (!CreateFactoryAndDevice(factoryFlags)) return false;
	if (!_renderer.Initialize(this, forWin)) return false;
	return true;
}

IRenderer& DX::Framework::GetRenderer()
{
	return _renderer;
}

ID3D12Device* DX::Framework::GetDevice()
{
	return _device.Get();
}

IDXGIFactory4* DX::Framework::GetFactory()
{
	return _factory.Get();
}

bool DX::Framework::CreateFactoryAndDevice(unsigned factoryFlags)
{
	RETURN_IF_FAILED(
		CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&_factory))
	);

	IDXGIAdapter1* gfxAdapter = ::GetHardwareAdapter(_factory.Get());

	RETURN_IF_FAILED(
		D3D12CreateDevice(gfxAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device))
	);

	MARK_SUCCESS;
}