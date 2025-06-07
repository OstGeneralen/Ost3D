// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#pragma once

namespace ost
{
	namespace math
	{
		struct Radians;

		struct Degrees
		{
			Degrees();
			Degrees(float v);
			Degrees(const Degrees&) = default;
			Degrees(Degrees&&) noexcept = default;

			float Value() const;

			operator Radians() const;

			Degrees& operator=(const Degrees& v);
			Degrees& operator=(float fv);
			Degrees& operator=(Radians rv);
		private:
			float _v;
		};

		struct Radians
		{
			Radians();
			Radians(float v);
			Radians(const Radians&) = default;
			Radians(Radians&&) noexcept = default;

			float Value() const;

			operator Degrees() const;

			Radians& operator=(const Radians& rv);
			Radians& operator=(float fv);
			Radians& operator=(Degrees dv);
		private:
			float _v;
		};

		// Literals _deg and _rad (for i.e radians = 180_deg or degrees = 3.141_rad)
		static Degrees operator "" _deg(long double v) { return Degrees(static_cast<float>(v)); }
		static Radians operator "" _rad(long double v) { return Radians(static_cast<float>(v)); }
	}
}