#pragma comment(lib, "Common_Debug")

#include <Common/Math/Mat3x3.h>
#include <Common/Math/Mat4x4.h>
#include <Common/Math/Vector4.h>
#include <Common/Math/Vector3.h>
#include <Common/Math/RotationUnits.h>
#include <Common/Utility/StringUtils.h>
#include <Common/Rendering/Color.h>
#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << "Vec3\n" << Util::StringConvert::ToString(Vector3f(1, 0, 2)) << std::endl;
	std::cout << "Vec4\n" << Util::StringConvert::ToString(Vector4f(1, 0, 2, 1)) << std::endl;
	std::cout << "Mat3x3\n" << Util::StringConvert::ToString(Mat3x3()) << std::endl;
	std::cout << "Mat4x4\n" << Util::StringConvert::ToString(Mat4x4()) << std::endl;

	return 0;
}