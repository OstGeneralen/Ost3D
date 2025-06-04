#pragma once
#include <Graphics/DX/DXCore.h>
#include <Graphics/DX/Renderer.h>
#include <Graphics/Rendering/RenderingFramework.h>

class Window;

namespace DX
{
	class Framework : public IRenderFramework
	{
	public:
		Framework();
		bool Initialize( const Window& forWindow ) override;
		IRenderer& GetRenderer() override;

		ID3D12Device* GetDevice();
		IDXGIFactory4* GetFactory();
	private:
		bool CreateFactoryAndDevice(unsigned factoryFlags);

	private:
#if _DEBUG
		ComPtr<ID3D12Debug1> _debugLayer;
#endif
		ComPtr<IDXGIFactory4> _factory;
		ComPtr<ID3D12Device> _device;

		Renderer _renderer;
	};
}