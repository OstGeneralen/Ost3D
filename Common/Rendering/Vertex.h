#pragma once
#include <Common/Math/Vector4.h>
#include <Common/Rendering/Color.h>

struct Vertex_Pos4_fCol4
{
	Vector4f Position;
	RGBAColor_f32 Color;
};

// Define the basic vertex type
using Vertex = Vertex_Pos4_fCol4;