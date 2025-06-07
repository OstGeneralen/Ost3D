#pragma once
#include <Engine/Math/Vector/Vector4.h>
#include <Engine/Rendering/Color/Color.h>

namespace ost
{
	struct Vertex_Pos4f_Col4f
	{
		Vector4f Position;
		RGBAColor_f32 Color;
	};
}