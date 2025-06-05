#include "ShaderCompiler.h"
#include <d3dcompiler.h>
#include <filesystem>
#include <sstream>

DX::ShaderCompiler::ShaderCompiler()
{
}

ShaderCompileInfo DX::ShaderCompiler::CompileShader(const char* path, EShaderType shaderType)
{
	std::stringstream errMsgStream;

	if (!std::filesystem::exists(path))
	{
		errMsgStream << "Shader File '" << path << "' could not be found";
		return ShaderCompileInfo{ false, errMsgStream.str(), CompiledShader{} };
	}

	// Evaluate and set compile flags from compiler options
	unsigned int compileFlags = 0u;
#if _DEBUG
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	LPCSTR shaderEntryPoint = "";
	LPCSTR shaderModelStr = "";

	switch (shaderType)
	{
	case EShaderType::Pixel:
		shaderEntryPoint = "PixelMain";
		shaderModelStr = "ps_5_1";
		break;
	case EShaderType::Vertex:
		shaderEntryPoint = "VertexMain";
		shaderModelStr = "vs_5_1";
		break;
	}

	std::string pathStr(path);
	std::wstring pathW(pathStr.begin(), pathStr.end());

	HRESULT compileResult = D3DCompileFromFile(
		pathW.c_str(),
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
		errorBlob->Release();
		return ShaderCompileInfo{ false, errMsg, CompiledShader() };
	}

	CompiledShader cs{ shaderType, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize()};
	return ShaderCompileInfo{ true, "", cs };
}
