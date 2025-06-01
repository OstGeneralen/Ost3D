#pragma comment(lib, "Common_Debug")

#include <Common/Math/Mat3x3.h>
#include <Common/Math/Mat4x4.h>
#include <Common/Math/Vector4.h>
#include <Common/Math/Vector3.h>
#include <Common/Math/RotationUnits.h>
#include <Common/Utility/StringUtils.h>
#include <iostream>

int main(int argc, char* argv[])
{

	Math::Vector3f v3{ 10,0,2 };
	Math::Vector4f v4{ 1,4,10,1 };

	Math::Mat3x3 m33 = Math::Mat3x3::XRotation(90.0_deg);
	Math::Mat4x4 m44 = Math::Mat4x4(m33, Math::Vector3f(1, 0, 1));

	std::cout << "Vector3 " << Util::StringConvert::ToString(v3) << std::endl;
	std::cout << "Vector4 " << Util::StringConvert::ToString(v4) << std::endl;
	std::cout << "Mat3\n" << Util::StringConvert::ToString(m33) << std::endl;
	std::cout << "Mat4\n" << Util::StringConvert::ToString(m44) << std::endl;


	return 0;
}