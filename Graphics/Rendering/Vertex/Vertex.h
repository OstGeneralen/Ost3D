#pragma once
#include <Common/Math/Vector4.h>
#include <Common/Rendering/Color.h>

using vertexFormatID_t = unsigned;

namespace VertexFormats
{
	constexpr static vertexFormatID_t INVALID = 0x00000000;
	constexpr static vertexFormatID_t fPos4_fCol4 = 0x00000001;
}

struct StaticVertexInfo
{
	void* InputElementsDesc{0};
	unsigned NumInputElements{0};
	vertexFormatID_t VertexFormatID{ VertexFormats::INVALID };
};

struct Vertex_Pos4_fCol4
{
	static StaticVertexInfo StaticVertexFormatInfo();
	static vertexFormatID_t StaticVertexFormatID();

	vertexFormatID_t VertexFormat() const;

	Vector4f Position;
	RGBAColor_f32 Color;
};

// Define the basic vertex type
using Vertex = Vertex_Pos4_fCol4;