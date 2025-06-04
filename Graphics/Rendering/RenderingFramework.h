#pragma once
#include <Graphics/Window/Window.h>
#include <Graphics/Rendering/Shader.h>
#include <Common/Rendering/Color.h>

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
};

class IRenderFramework
{
public:
	virtual ~IRenderFramework() = default;

	virtual bool Initialize(const Window& forWindow) = 0;
	virtual IRenderer& GetRenderer() = 0;
};

namespace Rendering
{
	extern IRenderFramework* CreateFramework();
	extern IShaderCompiler* CreateShaderCompiler();
}