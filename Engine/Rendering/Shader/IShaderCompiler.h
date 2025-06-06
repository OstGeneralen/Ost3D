#pragma once
#include <Engine/Rendering/Shader/Shader.h>

namespace ost
{
	class IShaderCompiler
	{
	public:
		virtual ~IShaderCompiler() = default;
		virtual ShaderCompileInfo CompileShader(const char* path, EShaderType shaderType) = 0;
	};
}