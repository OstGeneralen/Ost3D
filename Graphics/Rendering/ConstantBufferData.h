#pragma once
#include <Common/DataDefinitions.h>

template<typename T, size_t ALIGN = 255> 
struct TConstantBufferData
{
	T Content;
private:
	byte pad[ALIGN - sizeof(T)];
};