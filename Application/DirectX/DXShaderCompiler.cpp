#include "DXShaderCompiler.h"
#include <d3dcompiler.h>
#include <Common/Utility/Utilities.h>
#include <filesystem>

#define IF_TRUE( flag ) ? flag : 0

DXShaderCompileResult::DXShaderCompileResult(bool success, const std::string& errMsg, DXShader&& shader)
	: Success(success)
	, ErrorMessage(errMsg)
	, Shader(Util::Move(shader))
{
}

DXShaderCompiler::DXShaderCompiler(DXShaderInfo::EVersion shaderModel, EShaderCompileOption compilerOptions)
	: _vertexModelStr{ DXShaderInfo::ShaderModelStr(DXShaderInfo::EType::Vertex, shaderModel)}
	, _pixelModelStr{ DXShaderInfo::ShaderModelStr(DXShaderInfo::EType::Pixel, shaderModel) }
	, _compilerOptions(compilerOptions)
{

}

DXShaderCompileResult DXShaderCompiler::CompileShader(const std::wstring& path, DXShaderInfo::EType shaderType)
{
	unsigned int compileFlags = 0u;
	
	if (!std::filesystem::exists(path))
	{
		DXShader emptyShader{};
		std::string msg = std::string("Shader Compile Error: Shader file could not be found: ") + std::string(path.begin(), path.end());
		return Util::Move(DXShaderCompileResult(false, msg, Util::Forward(emptyShader)));
	}

	// Evaluate and set compile flags from compiler options
	const Flags<EShaderCompileOption> cf = FlagsConvert(_compilerOptions);
	compileFlags |= cf.Has(EShaderCompileOption::Debug)			IF_TRUE( D3DCOMPILE_DEBUG );
	compileFlags |= cf.Has(EShaderCompileOption::SkipOptimize)	IF_TRUE( D3DCOMPILE_SKIP_OPTIMIZATION );

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	LPCSTR shaderEntryPoint = "";
	LPCSTR shaderModelStr = "";

	switch (shaderType)
	{
	case DXShaderInfo::EType::Pixel:
		shaderEntryPoint = "PixelMain";
		shaderModelStr = _pixelModelStr.c_str();
		break;
	case DXShaderInfo::EType::Vertex:
		shaderEntryPoint = "VertexMain";
		shaderModelStr = _vertexModelStr.c_str();
		break;
	}

	HRESULT compileResult = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		shaderEntryPoint,
		shaderModelStr,
		compileFlags,
		0,
		&shaderBlob,
		&errorBlob
	);

	if (compileResult != S_OK)
	{
		std::string errMsg = static_cast<char*>(errorBlob->GetBufferPointer());
		DXShader emptyShader{};
		errorBlob->Release();
		return Util::Move(DXShaderCompileResult(false, errMsg, Util::Forward(emptyShader)));
	}

	DXShader compiledShader{ shaderType, shaderBlob };
	return Util::Move(DXShaderCompileResult(true, "", Util::Forward(compiledShader)));
}


