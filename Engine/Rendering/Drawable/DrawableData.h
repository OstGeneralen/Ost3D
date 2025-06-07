#pragma once
#include <Engine/Rendering/Drawable/Vertex/Vertex.h>

namespace ost
{
	struct DrawableData
	{
		Vertex_Pos4f_Col4f Vertices[3] = {
			{ { -0.5f ,-0.5f, 0.0f, 1.0f }, RGBAColor_f32{0xFF0000FF}},
			{ { 0.0f , 0.5f, 0.0f, 1.0f }, RGBAColor_f32{0x0000FFFF}},
			{ { 0.5f , -0.5f, 0.0f, 1.0f }, RGBAColor_f32{0x00FF00FF}}
		};
		const uint16 NumVerts = 3;
	};
}