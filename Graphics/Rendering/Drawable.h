#pragma once
#include <Common/Rendering/Vertex.h>

struct IDrawable
{
	virtual void* DataStartAddress() = 0;
	virtual size_t ByteSize() = 0;
	virtual size_t StrideSize() = 0;
	virtual size_t VertexCount() = 0;
};

struct VertexDrawable : public IDrawable
{
public:
	VertexDrawable& SetVertexCount(size_t numVerts);
	VertexDrawable& AddVertex(const Vertex& v);

	void* DataStartAddress() override;
	size_t ByteSize() override;
	size_t StrideSize() override;
	size_t VertexCount() override;
private:
	size_t _vertCount;
	size_t _nextWriteVert;
	Vertex* _vertices = nullptr;
};