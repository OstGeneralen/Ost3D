#pragma once
#include <Graphics/Rendering/RenderingFramework.h>

namespace DX
{
	class ShaderCompiler : public IShaderCompiler
	{
	public:
		ShaderCompiler();
		virtual ShaderCompileInfo CompileShader(const char* path, EShaderType shaderType) override;
	};
}