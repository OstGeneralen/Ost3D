#pragma once
#include <Engine/Rendering/Shader/IShaderCompiler.h>
#include <Engine/Rendering/Shader/Shader.h>

namespace ost
{
	namespace dx
	{
		class ShaderCompiler : public IShaderCompiler
		{
		public:
			ShaderCompiler();
			virtual ShaderCompileInfo CompileShader(const char* path, EShaderType shaderType) override;
		};
	}
}