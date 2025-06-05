#pragma once
#include <Graphics/Window/Window.h>
#include <Graphics/Rendering/Shader.h>
#include <Common/Rendering/Color.h>
#include <Graphics/Rendering/RenderState.h>
#include <Graphics/Rendering/ConstantBuffer.h>

class IShaderCompiler
{
public:
	virtual ~IShaderCompiler() = default;
	virtual ShaderCompileInfo CompileShader(const char* path, EShaderType shaderType) = 0;
};

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual void BeginRenderFrame(RGBAColor_u8 clearColor) = 0;
	virtual void EndRenderFrame() = 0;
	virtual void PresentFrame() = 0;

	virtual void ExecuteRenderState(const RenderStateHandle& handle) = 0;

	virtual RenderStateHandle CreateRenderState(const RenderStateDesc& desc) = 0;
};

class IRenderFramework
{
public:
	virtual ~IRenderFramework() = default;

	virtual bool Initialize(const Window& forWindow) = 0;
	virtual IRenderer& GetRenderer() = 0;

	virtual bool MakeConstantBuffer(ConstantBuffer& buffer, unsigned bindRegister) = 0;
};

namespace Rendering
{
	extern IRenderFramework* CreateFramework();
	extern IShaderCompiler* CreateShaderCompiler();
}