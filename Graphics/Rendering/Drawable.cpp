#include "Drawable.h"

VertexDrawable& VertexDrawable::SetVertexCount(size_t numVerts)
{
    if (_vertices != nullptr)
    {
        delete[] _vertices;
    }

    _vertCount = numVerts;
    _vertices = new Vertex[numVerts];
    _nextWriteVert = 0;
    return *this;
}

VertexDrawable& VertexDrawable::AddVertex(const Vertex& v)
{
    if (_nextWriteVert == _vertCount) {} // error
    _vertices[_nextWriteVert] = v;
    _nextWriteVert++;
    return *this;
}

void* VertexDrawable::DataStartAddress()
{
    return _vertices;
}

size_t VertexDrawable::ByteSize()
{
    return sizeof(Vertex) * _vertCount;
}

size_t VertexDrawable::StrideSize()
{
    return sizeof(Vertex);
}

size_t VertexDrawable::VertexCount()
{
    return _vertCount;
}
