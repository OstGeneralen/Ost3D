#pragma once
#include <DirectX/DXShaderInfo.h>

struct DXShader
{
public:
	DXShader();
	DXShader(DXShaderInfo::EType shaderType, void* compiledCodePtr);
	DXShader(const DXShader&) = delete;
	DXShader(DXShader&& s) noexcept;
	~DXShader();

	void operator=(DXShader&& s) noexcept;

	void Release();
	bool IsValid() const;

private:
	DXShaderInfo::EType _shaderType;
	void* _compiledCodePtr;
};