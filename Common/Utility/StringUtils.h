#pragma once
#include <string>
#include <Common/Rendering/Color.h>

namespace Math
{
	class Vector3f;
	class Vector4f;
	class Mat4x4;
	class Mat3x3;
}


namespace Util
{
	class StringConvert
	{
	public:
		static std::string ToString(const Math::Vector3f& v3);
		static std::string ToString(const Math::Vector4f& v4);
		
		static std::string ToString(const Math::Mat3x3& m33);
		static std::string ToString(const Math::Mat4x4& m44);

		static std::string ToString(const RGBAColor_f32& c);
		static std::string ToString(const RGBAColor_u8& c);
	};
}