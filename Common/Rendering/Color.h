#pragma once
#include <Common/DataDefinitions.h>

struct RGBAColor_f32;
struct RGBAColor_u8;

// ------------------------------------------------------------
// float RGBA
// ------------------------------------------------------------

struct RGBAColor_f32
{
	float R;
	float G;
	float B;
	float A;

	RGBAColor_f32();
	RGBAColor_f32(float r, float g, float b);
	RGBAColor_f32(float r, float g, float b, float a);
	RGBAColor_f32(const RGBAColor_f32&);
	RGBAColor_f32(uint32 hex);

	void CopyToFloatBuffer(float* buffer) const;

	RGBAColor_u8 ToRGBA_u8() const;
};

// ------------------------------------------------------------
// uint8 RGBA
// ------------------------------------------------------------

struct RGBAColor_u8
{
	byte R;
	byte G;
	byte B;
	byte A;

	RGBAColor_u8();
	RGBAColor_u8(byte r, byte g, byte b, byte a = 255u);
	RGBAColor_u8(uint32 hex);

	RGBAColor_f32 ToRGBA_f32() const;
};