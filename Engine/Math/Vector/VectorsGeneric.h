#pragma once
#include <Engine/Math/CommonMath.h>

// ------------------------------------------------------------
// Vector 2
// ------------------------------------------------------------
namespace ost
{
	template<typename T>
	struct TVector2
	{
		T X, Y;

		// ------------------------------------------------------------
		// Ctor
		// ------------------------------------------------------------	
		TVector2() : X{ static_cast<T>(0) }, Y{ static_cast<T>(0) } {}
		TVector2(T xy) : X{ xy }, Y{ xy } {}
		TVector2(T x, T y) : X{ x }, Y{ y } {}
		TVector2(const TVector2& v) : X{ v.X }, Y{ v.Y } {}

		// ------------------------------------------------------------
		// Ops
		// ------------------------------------------------------------
		TVector2& operator=(const TVector2& v) { X = v.X; Y = v.Y; return *this; }

		TVector2& operator+=(const TVector2& v) { X += v.X; Y += v.Y; return *this; }
		TVector2& operator-=(const TVector2& v) { X -= v.X; Y -= v.Y; return *this; }
		TVector2& operator*=(T s) { X *= s; Y *= s; return *this; }
		TVector2& operator/=(T s) { X /= s; Y /= s; return *this; }

		TVector2 operator+(const TVector2& v) const { return TVector2(X + v.X, Y + v.Y); }
		TVector2 operator-(const TVector2& v) const { return TVector2(X - v.X, Y - v.Y); }
		TVector2 operator*(T s) const { return TVector2(X * s, Y * s); }
		TVector2 operator/(T s) const { return TVector2(X / s, Y / s); }

		bool operator==(const TVector2& v) const { return math::EqualsT<T>(X, v.X) && math::EqualsT<T>(Y, v.Y); }
	};

	// ------------------------------------------------------------
	// Vector 3
	// ------------------------------------------------------------

	template<typename T>
	struct TVector3
	{
		T X, Y, Z;

		// ------------------------------------------------------------
		// Ctor
		// ------------------------------------------------------------	
		TVector3() : X{ static_cast<T>(0) }, Y{ static_cast<T>(0) }, Z{ static_cast<T>(0) } {}
		TVector3(T xyz) : X{ xyz }, Y{ xyz }, Z{ xyz } {}
		TVector3(T x, T y, T z) : X{ x }, Y{ y }, Z{ z } {}
		TVector3(const TVector3& v) : X{ v.X }, Y{ v.Y }, Z{ v.Z } {}

		// ------------------------------------------------------------
		// Ops
		// ------------------------------------------------------------
		TVector3& operator=(const TVector3& v) { X = v.X; Y = v.Y; Z = v.Z; return *this; }

		TVector3& operator+=(const TVector3& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
		TVector3& operator-=(const TVector3& v) { X -= v.X; Y -= v.Y; Z -= v.Z; return *this; }
		TVector3& operator*=(T s) { X *= s; Y *= s; Z *= s; return *this; }
		TVector3& operator/=(T s) { X /= s; Y /= s; Z /= s; return *this; }

		TVector3 operator+(const TVector3& v) const { return TVector3(X + v.X, Y + v.Y, Z + v.Z); }
		TVector3 operator-(const TVector3& v) const { return TVector3(X - v.X, Y - v.Y, Z - v.Z); }
		TVector3 operator*(T s) const { return TVector3(X * s, Y * s, Z * s); }
		TVector3 operator/(T s) const { return TVector3(X / s, Y / s, Z / s); }

		bool operator==(const TVector3& v) const { return math::EqualsT<T>(X, v.X) && math::EqualsT<T>(Y, v.Y) && math::EqualsT<T>(Z, v.Z); }
	};

	// ------------------------------------------------------------
	// Typedefs
	// ------------------------------------------------------------

	using Vec2i = TVector2<int>;
	using Vec3i = TVector3<int>;
	using Dimensions = Vec2i;

	using Vec2u = TVector2<unsigned>;
	using Vec3u = TVector3<unsigned>;
}