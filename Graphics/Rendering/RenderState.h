#pragma once
#include <Graphics/Rendering/Shader.h>
#include <Graphics/Rendering/Drawable.h>

struct RenderStateDesc
{
	CompiledShader VertexShader = {};
	CompiledShader PixelShader = {};
};

class IRenderState
{
public:
	virtual ~IRenderState() = default;

	virtual void Initialize(const RenderStateDesc& desc) = 0;
	virtual void AddDrawable(IDrawable* drawable) = 0;
	virtual void Execute() = 0;
};