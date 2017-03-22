#pragma once

#include "Core.hpp"
#include "Vector4.hpp"

namespace ft
{
	// fw
	struct Color32;

	struct Color : public Vector4
	{
		Color();
		Color(const Color32& oColor32);
		virtual ~Color();

		operator Color32() const;
	};


	struct Color32
	{
		union
		{
			struct { uint8 r, g, b, a; };
			uint32 m_iColor;
		};

		Color32();
		Color32(uint32 iR, uint32 iG, uint32 iB, uint32 iA = 255);
		Color32(uint32 iColor);
		Color32(const Color& oColor);
		~Color32();

		inline operator Color() const							{ return Color(*this); }
		inline operator uint32() const							{ return m_iColor; }

		inline Color32	operator + (const Color32& oColor)		{ return Color32(r + oColor.r, g + oColor.g, b + oColor.b, a + oColor.a); }
		inline Color32	operator - (const Color32& oColor)		{ return Color32(r - oColor.r, g - oColor.g, b - oColor.b, a - oColor.a); }
		inline Color32	operator * (float f)					{ return Color32((uint32)(r * f), (uint32)(g * f), (uint32)(b * f), (uint32)(a * f)); }

		inline void		Set(uint32 iColor)						{ m_iColor = iColor; }
	};

	Color		ClampedAdd(const Color& c1, const Color& c2);
	Color32		ClampedAdd(const Color32& c1, const Color32& c2);
	Color		ClampedSub(const Color& c1, const Color& c2);
	Color32		ClampedSub(const Color32& c1, const Color32& c2);
	Color		ClampedMult(const Color& c1, float f);
	Color32		ClampedMult(const Color32& c1, float f);
}
