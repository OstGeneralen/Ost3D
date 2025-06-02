#pragma once
#include <string>

namespace DXShaderInfo
{
	enum class EType
	{
		None,
		Pixel,
		Vertex,
	};

	enum class EVersion
	{
		Unknown,	// Error
		General,	// 5.1
		DX12,		// 6.0
		RayTrace,	// 6.5
	};

	extern std::string ShaderModelStr(EType type, EVersion model);
}