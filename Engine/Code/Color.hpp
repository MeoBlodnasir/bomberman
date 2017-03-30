#pragma once

#include "Vector3.hpp"
#include "Vector4.hpp"

namespace ft
{
	// fw
	struct Color3;
	struct Color32;

	struct Color : public Vector4
	{
		Color();
		explicit Color(float fR, float fG, float fB, float fA = 1.f);
		Color(const Color3& oColor3);
		Color(const Color32& oColor32);
		~Color() {}

		operator Color3() const;
		operator Color32() const;
	};

	struct Color3 : public Vector3
	{
		Color3();
		explicit Color3(float fR, float fG, float fB);
		Color3(const Color& oColor);
		Color3(const Color32& oColor32);
		~Color3() {}

		operator Color() const;
		operator Color32() const;
	};

	struct Color32
	{
		union
		{
			struct { uint8 r, g, b, a; };
			uint32 iColor;
		};

		Color32();
		explicit Color32(uint32 iR, uint32 iG, uint32 iB, uint32 iA = 255);
		Color32(uint32 iColor);
		Color32(const Color& oColor);
		Color32(const Color3& oColor3);
		~Color32() {}

		inline operator Color() const							{ return Color(*this); }
		inline operator Color3() const							{ return Color3(*this); }
		inline operator uint32() const							{ return iColor; }

		inline Color32	operator + (const Color32& oColor)		{ return Color32(r + oColor.r, g + oColor.g, b + oColor.b, a + oColor.a); }
		inline Color32	operator - (const Color32& oColor)		{ return Color32(r - oColor.r, g - oColor.g, b - oColor.b, a - oColor.a); }
		inline Color32	operator * (float f)					{ return Color32((uint32)(r * f), (uint32)(g * f), (uint32)(b * f), (uint32)(a * f)); }
	};
}
