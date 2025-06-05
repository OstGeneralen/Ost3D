#include "Drawable.h"

unsigned Drawable::ByteSize() const
{
	return VertexSize * VertexCount;
}

