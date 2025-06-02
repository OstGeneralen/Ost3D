#pragma once
#include <string>
#include <Common/Utility/EnumFlag.h>
#include "DirectX/DXShader.h"
#include "DirectX/DXShaderInfo.h"

struct DXShaderCompileResult
{
	DXShaderCompileResult() = delete;
	DXShaderCompileResult(bool success, const std::string& errMsg, DXShader&& shader);
	DXShaderCompileResult(const DXShaderCompileResult&) = delete;
	DXShaderCompileResult(DXShaderCompileResult&&) noexcept = default;
	~DXShaderCompileResult() = default;

	bool Success;
	std::string ErrorMessage;
	DXShader Shader;
};


enum class EShaderCompileOption
{
	None			= 0,
	Debug			= 1 << 0,
	SkipOptimize	= 1 << 1,
};
IS_FLAGS(EShaderCompileOption);


class DXShaderCompiler
{

public:
	DXShaderCompiler( DXShaderInfo::EVersion shaderModel, EShaderCompileOption compilerOptions );
	DXShaderCompileResult CompileShader(const std::wstring& path, DXShaderInfo::EType shaderType);

private:
	std::string _vertexModelStr;
	std::string _pixelModelStr;
	EShaderCompileOption _compilerOptions;
};