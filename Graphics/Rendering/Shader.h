#pragma once
#include <string>

enum class EShaderType
{
	Unknown,
	Vertex,
	Pixel,
};

struct CompiledShader
{
	bool Valid() const 
	{
		return ShaderType != EShaderType::Unknown && CompiledData != nullptr && CompiledDataSize != 0u;
	}

	EShaderType ShaderType = EShaderType::Unknown;
	void* CompiledData = nullptr;
	size_t CompiledDataSize = 0u;
};

struct ShaderCompileInfo
{
	bool Status = true;
	std::string ErrorMessage;
	CompiledShader Shader;
};
