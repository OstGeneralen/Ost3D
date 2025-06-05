#pragma once
#include <Graphics/Rendering/Shader.h>
#include <Graphics/Rendering/Drawable.h>
#include <Graphics/Rendering/Vertex/Vertex.h>
#include <Graphics/Rendering/ConstantBuffer.h>

struct RenderStateDesc
{
	CompiledShader VertexShader = {};
	CompiledShader PixelShader = {};
	StaticVertexInfo VertexFormatInfo = {};
	bool HasConstantBuffer{false};
};

class IRenderState
{
public:
	virtual ~IRenderState() = default;
	virtual bool AddDrawable(const Drawable& drawable) = 0;
	virtual void SetConstantBuffer(ConstantBuffer* buffer) = 0;
};

struct RenderStateHandle
{
public:
	RenderStateHandle() = default;
	RenderStateHandle(IRenderState& stateRef, size_t stateID) : _stateID{ stateID }, _statePtr{ &stateRef } {}
	bool AddDrawable(const Drawable& d) { return _statePtr->AddDrawable(d); }
	void SetConstBuffer(ConstantBuffer* b) { _statePtr->SetConstantBuffer(b); }

	size_t GetStateID() const { return _stateID; }
private:
	size_t _stateID;
	IRenderState* _statePtr;
};