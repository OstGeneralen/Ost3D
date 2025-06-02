#include "StringUtils.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Math/Mat3x3.h"
#include "../Math/Mat4x4.h"
#include "../Math/CommonMath.h"
#include <iomanip>

#include <sstream>


std::string Util::StringConvert::ToString(const Math::Vector3f& v3)
{
	std::stringstream sstream;


	sstream << "(" << 
		std::fixed << std::setprecision(2) << v3.X << ", " <<
		std::fixed << std::setprecision(2) << v3.Y << ", " <<
		std::fixed << std::setprecision(2) << v3.Z << ")";
	return sstream.str();
}

std::string Util::StringConvert::ToString(const Math::Vector4f& v4)
{
	std::stringstream sstream;

	sstream << "(" << 
		std::fixed << std::setprecision(2) << v4.X << ", " <<
		std::fixed << std::setprecision(2) << v4.Y << ", " <<
		std::fixed << std::setprecision(2) << v4.Z << ", " <<
		std::fixed << std::setprecision(2) << v4.W << ")";
	return sstream.str();
}

std::string Util::StringConvert::ToString(const Math::Mat3x3& m33)
{
	std::stringstream sstream;

	auto r1 = m33.Row1();
	auto r2 = m33.Row2();
	auto r3 = m33.Row3();

	sstream <<  std::fixed << std::setprecision(2) << 
		r1.X << "    " << 
		r1.Y << "    " <<
		r1.Z << "\n\n";
	
	sstream << std::fixed << std::setprecision(2) << 
		r2.X << "    " << 
		r2.Y << "    " << 
		r2.Z << "\n\n";

	sstream << std::fixed << std::setprecision(2) << 
		r3.X << "    " << 
		r3.Y << "    " <<
		r3.Z;

	return sstream.str();
}

std::string Util::StringConvert::ToString(const Math::Mat4x4& m44)
{
	std::stringstream sstream;

	auto r1 = m44.Row1();
	auto r2 = m44.Row2();
	auto r3 = m44.Row3();
	auto r4 = m44.Row4();

	sstream << std::fixed << std::setprecision(2) << 
		r1.X << "    " <<
		r1.Y << "    " <<
		r1.Z << "    " <<
		r1.W << "\n\n";
	
	sstream << std::fixed << std::setprecision(2) << 
		r2.X << "    " <<
		r2.Y << "    " <<
		r2.Z << "    " <<
		r2.W << "\n\n";

	sstream << std::fixed << std::setprecision(2) << 
		r3.X << "    " <<
		r3.Y << "    " <<
		r3.Z << "    " <<
		r3.W << "\n\n";
		//"- - -:- - - : - - -: - - " << "\n";
	
	sstream << std::fixed << std::setprecision(2) << 
		r4.X << "    " <<
		r4.Y << "    " <<
		r4.Z << "    " <<
		r4.W << "\n";
		

	return sstream.str();
}

std::string Util::StringConvert::ToString(const RGBAColor_f32& c)
{
	std::stringstream sstream;

	sstream <<
		"[R: "	<< std::fixed << std::setprecision(2) << c.R <<
		", G: " << std::fixed << std::setprecision(2) << c.G <<
		", B: " << std::fixed << std::setprecision(2) << c.B <<
		" A: "	<< std::fixed << std::setprecision(2) << c.A << 
		']';
	return sstream.str();
}

std::string Util::StringConvert::ToString(const RGBAColor_u8& c)
{
	std::stringstream sstream;

	sstream << 
		"[R: " << static_cast<unsigned>(c.R) << 
		", G: " << static_cast<unsigned>(c.G) << 
		", B: " << static_cast<unsigned>(c.B) << 
		" A: " << static_cast<unsigned>(c.A) << ']';
	return sstream.str();
}

