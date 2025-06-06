#include "Color.h"

#include <Engine/Utility/Memory/MemUtils.h>

using namespace ost;

// ------------------------------------------------------------
// float RGBA
// ------------------------------------------------------------

RGBAColor_f32::RGBAColor_f32()
	: RGBAColor_f32(0.0f, 0.0f, 0.0f, 1.0f)
{
}

RGBAColor_f32::RGBAColor_f32(float r, float g, float b)
	: RGBAColor_f32(r, g, b, 1.0f)
{
}

RGBAColor_f32::RGBAColor_f32(float r, float g, float b, float a)
	: R{r}
	, G{g}
	, B{b}
	, A{a}
{
}

RGBAColor_f32::RGBAColor_f32(const RGBAColor_f32& c)
	: R{c.R}
	, G{c.G}
	, B{c.B}
	, A{c.A}
{
}

RGBAColor_f32::RGBAColor_f32(uint32 hex)
	: RGBAColor_f32(0.f, 0.f, 0.f, 1.f)
{
	RGBAColor_f32 v = RGBAColor_u8{ hex }.ToRGBA_f32();
	R = v.R;
	G = v.G;
	B = v.B;
	A = v.A;
}

void RGBAColor_f32::CopyToFloatBuffer(float* buffer) const
{
	MemCopy(buffer, this, sizeof(float) * 4);
}

RGBAColor_u8 RGBAColor_f32::ToRGBA_u8() const
{
	return RGBAColor_u8{
		static_cast<byte>(R * 255.0f),
		static_cast<byte>(G * 255.0f),
		static_cast<byte>(B * 255.0f),
		static_cast<byte>(A * 255.0f) };
}

// ------------------------------------------------------------
// uint8 RGBA
// ------------------------------------------------------------

RGBAColor_u8::RGBAColor_u8()
	: RGBAColor_u8(0u, 0u, 0u, 255u)
{
}

RGBAColor_u8::RGBAColor_u8(byte r, byte g, byte b, byte a)
	: R{r}
	, G{g}
	, B{b}
	, A{a}
{
}

RGBAColor_u8::RGBAColor_u8(uint32 hex)
	: RGBAColor_u8(
		(hex & 0xFF000000) >> 24,
		(hex & 0x00FF0000) >> 16,
		(hex & 0x0000FF00) >> 8,
		(hex & 0x000000FF) >> 0)
{
}

RGBAColor_f32 RGBAColor_u8::ToRGBA_f32() const
{
	return RGBAColor_f32(
		static_cast<float>(R) / 255.0f,
		static_cast<float>(G) / 255.0f,
		static_cast<float>(B) / 255.0f,
		static_cast<float>(A) / 255.0f
	);
}


