#include "DXShader.h"
#include <d3d12.h>
#include <Common/Utility/Utilities.h>

DXShader::DXShader()
	: _shaderType{ DXShaderInfo::EType::None }
	, _compiledCodePtr{ nullptr }
{
}

DXShader::DXShader(DXShaderInfo::EType shaderType, void* compiledCodePtr)
	: _shaderType{shaderType}
	, _compiledCodePtr{compiledCodePtr}
{
}

DXShader::DXShader(DXShader&& s) noexcept
	: _shaderType{s._shaderType}
	, _compiledCodePtr{s._compiledCodePtr}
{
	// Invalidate the moved from instance
	s._shaderType = DXShaderInfo::EType::None;
	s._compiledCodePtr = nullptr;
}

DXShader::~DXShader()
{
	Release();
}

void DXShader::operator=(DXShader&& s) noexcept
{
	Release();
	Util::Swap(_compiledCodePtr, s._compiledCodePtr);
	Util::Swap(_shaderType, s._shaderType);
}

void DXShader::Release()
{
	if (_compiledCodePtr)
	{
		static_cast<ID3DBlob*>(_compiledCodePtr)->Release();
		_compiledCodePtr = nullptr;
	}
}

bool DXShader::IsValid() const
{
	return _shaderType != DXShaderInfo::EType::None && _compiledCodePtr != nullptr;
}
