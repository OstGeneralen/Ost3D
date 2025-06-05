#pragma once
#include <Graphics/Rendering/Vertex/Vertex.h>

struct Drawable
{
	vertexFormatID_t VertexFormatID;
	unsigned VertexCount;
	unsigned VertexSize;
	void* VertexDataBuffer;
	unsigned ByteSize() const;
};