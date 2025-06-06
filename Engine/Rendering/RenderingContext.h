#pragma once
#include <Engine/Rendering/Shader/IShaderCompiler.h>

namespace ost
{
	struct RenderingContext
	{
		RenderingContext(IShaderCompiler& compiler)
			: ShaderCodeCompiler{ compiler }
		{
		}
		RenderingContext(RenderingContext&& c) noexcept = default;


		IShaderCompiler& ShaderCodeCompiler;
	};
}