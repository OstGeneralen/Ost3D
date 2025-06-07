#pragma once
#include <Engine/Rendering/RenderState.h>

namespace ost
{
	class RenderingContext
	{
	public:
		virtual ~RenderingContext() = default;

		virtual RenderState CreateRenderState(const RenderStateDesc& desc) = 0;
		virtual void ReleaseRenderState(RenderState& state) = 0;

		virtual void SetActiveRenderState(RenderState& state) = 0;
	};
}