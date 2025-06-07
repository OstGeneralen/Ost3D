// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#pragma once

#define NO_COPY( T ) T( const T& ) = delete; T& operator=(const T&) = delete;
#define NO_MOVE( T ) T( T&& ) noexcept = delete; T& operator=(T&&) noexcept = delete;

namespace ost
{
	using uint8 = unsigned char;
	using int8 = signed char;
	using byte = uint8;

	using uint16 = unsigned short;
	using int16 = signed short;

	using uint32 = unsigned int;
	using int32 = signed int;

	using uint64 = unsigned long long;
	using int64 = signed long long;

	using float32 = float;
	using float64 = long double;
}