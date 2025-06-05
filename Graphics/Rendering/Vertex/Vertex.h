#pragma once
#include <Common/Math/Vector4.h>
#include <Common/Rendering/Color.h>

struct Vertex_Pos4_fCol4
{
public:
	Vector4f Position;
	RGBAColor_f32 Color;

public:
	Vertex_Pos4_fCol4() = default;
	Vertex_Pos4_fCol4(const Vertex_Pos4_fCol4&) = default;
	Vertex_Pos4_fCol4(const Vector4f& p, const RGBAColor_f32& c) : Position{p} , Color{c} { }

	Vertex_Pos4_fCol4& operator=(const Vertex_Pos4_fCol4&) = default;
};

// Define the basic vertex type
using Vertex = Vertex_Pos4_fCol4;