#include "StringUtils.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Math/Mat3x3.h"
#include "../Math/Mat4x4.h"
#include "../Math/CommonMath.h"

#include <sstream>


std::string Util::StringConvert::ToString(const Math::Vector3f& v3)
{
	std::stringstream sstream;
	float x = Math::FltEquals(v3.X, 0.0f) ? 0 : v3.X;
	float y = Math::FltEquals(v3.Y, 0.0f) ? 0 : v3.Y;
	float z = Math::FltEquals(v3.Z, 0.0f) ? 0 : v3.Z;

	sstream << "(" << x << ", " << y << ", " << z << ")";
	return sstream.str();
}

std::string Util::StringConvert::ToString(const Math::Vector4f& v4)
{
	std::stringstream sstream;
	float x = Math::FltEquals(v4.X, 0.0f) ? 0 : v4.X;
	float y = Math::FltEquals(v4.Y, 0.0f) ? 0 : v4.Y;
	float z = Math::FltEquals(v4.Z, 0.0f) ? 0 : v4.Z;
	float w = Math::FltEquals(v4.W, 0.0f) ? 0 : v4.W;

	sstream << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return sstream.str();
}

std::string Util::StringConvert::ToString(const Math::Mat3x3& m33)
{
	std::stringstream sstream;

	auto r1 = m33.Row1();
	auto r2 = m33.Row2();
	auto r3 = m33.Row3();

	sstream << r1.X << "\t\t" << r1.Y << "\t\t" << r1.Z << "\n";
	sstream << r2.X << "\t\t" << r2.Y << "\t\t" << r2.Z << "\n";
	sstream << r3.X << "\t\t" << r3.Y << "\t\t" << r3.Z;

	return sstream.str();
}

std::string Util::StringConvert::ToString(const Math::Mat4x4& m44)
{
	std::stringstream sstream;

	auto r1 = m44.Row1();
	auto r2 = m44.Row2();
	auto r3 = m44.Row3();
	auto r4 = m44.Row4();

	sstream << r1.X << "\t\t" << r1.Y << "\t\t" << r1.Z << "\t\t" << r1.W << "\n";
	sstream << r2.X << "\t\t" << r2.Y << "\t\t" << r2.Z << "\t\t" << r2.W << "\n";
	sstream << r3.X << "\t\t" << r3.Y << "\t\t" << r3.Z << "\t\t" << r3.W << "\n";
	sstream << r4.X << "\t\t" << r4.Y << "\t\t" << r4.Z << "\t\t" << r4.W << "\n";

	return sstream.str();
}

