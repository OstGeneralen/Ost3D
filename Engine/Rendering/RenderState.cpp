#include "RenderState.h"
#include <Engine/Rendering/DX/DXCore.h>

ost::uint32 ost::RenderInputAlignmentToBytes(ERenderInputAlignment alignment)
{
	switch (alignment)
	{
	case ost::ERenderInputAlignment::f32_RGBA: return sizeof(float) * 4;
	case ost::ERenderInputAlignment::f32_RGB: return sizeof(float) * 3;
	case ost::ERenderInputAlignment::f32_RG: return sizeof(float) * 2;
	case ost::ERenderInputAlignment::f32_R: return sizeof(float);
	}
	return 0;
}

namespace
{
	DXGI_FORMAT AlignmentEnumToDXGI(ost::ERenderInputAlignment a)
	{
		switch (a)
		{
		case ost::ERenderInputAlignment::f32_RGBA: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ost::ERenderInputAlignment::f32_RGB: return DXGI_FORMAT_R32G32B32_FLOAT;
		case ost::ERenderInputAlignment::f32_RG: return DXGI_FORMAT_R32G32_FLOAT;
		case ost::ERenderInputAlignment::f32_R: return DXGI_FORMAT_R32_FLOAT;
		}
		return DXGI_FORMAT_UNKNOWN;
	}
}

ost::ShaderCompileInfo ost::RenderStateDesc::CompileVertexShader(IShaderCompiler& compiler) const
{
	return compiler.CompileShader(_vertexShaderPath.c_str(), EShaderType::Vertex);
}

ost::ShaderCompileInfo ost::RenderStateDesc::CompilePixelShader(IShaderCompiler& compiler) const
{
	return compiler.CompileShader(_pixelShaderPath.c_str(), EShaderType::Pixel);
}

void ost::RenderStateDesc::PopulateInputElementsDesc(void** buffer) const
{
	D3D12_INPUT_ELEMENT_DESC* descs = static_cast<D3D12_INPUT_ELEMENT_DESC*>(*buffer);
	for (size_t i = 0; i < _inputElements.Size(); ++i)
	{
		auto& e = _inputElements[i];
		descs[i].SemanticName = e.Name.c_str();
		descs[i].Format = ::AlignmentEnumToDXGI(e.Alignment);
		descs[i].AlignedByteOffset = e.Offset;
		descs[i].InputSlot = e.Slot;
		descs[i].InstanceDataStepRate = e.Step;
		descs[i].SemanticIndex = e.Idx;
		descs[i].InputSlotClass = e.PerVertex ? D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA : D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
	}
}

ost::uint32 ost::RenderStateDesc::GetInputElementNum() const
{
	return _inputElements.Size();
}

void ost::RenderStateDesc::AddInputElement(
	const char* name, 
	ERenderInputAlignment alignment, 
	bool perVertex, 
	uint32 idx, uint32 slot, uint32 step)
{
	_inputElements.Add({ name, idx, alignment, slot, _currentNextInputElementOffset, perVertex, step });
	_currentNextInputElementOffset += RenderInputAlignmentToBytes(alignment);
}

void ost::RenderStateDesc::SetShaderPath(const char* path, EShaderType shaderType)
{
	switch (shaderType)
	{
	case ost::EShaderType::Vertex:	_vertexShaderPath = path;	break;
	case ost::EShaderType::Pixel:	_pixelShaderPath = path;	break;
	}
}