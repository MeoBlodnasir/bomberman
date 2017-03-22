
#include "Color.hpp"

namespace ft
{
	Color::Color()
	{
	}

	Color::Color(const Color32& oColor32)
	{
		const float fInv = 1.f / 255.f;
		r = (float)oColor32.r * fInv;
		g = (float)oColor32.g * fInv;
		b = (float)oColor32.b * fInv;
		a = (float)oColor32.a * fInv;
	}

	Color::operator Color32() const
	{
		return Color32(*this);
	}

	Color32::Color32()
		: m_iColor(0)
	{
	}

	Color32::Color32(uint32 iR, uint32 iG, uint32 iB, uint32 iA /*= 255*/)
		: r((uint8)iR)
		, g((uint8)iG)
		, b((uint8)iB)
		, a((uint8)iA)
	{
	}

	Color32::Color32(uint32 iColor)
		: m_iColor(iColor)
	{
	}

	Color32::Color32(const Color& oColor)
		: r((uint8)oColor.r)
		, g((uint8)oColor.g)
		, b((uint8)oColor.b)
		, a((uint8)oColor.a)
	{
	}

	Color32::~Color32()
	{
	}

	Color		ClampedAdd(const Color& /*c1*/, const Color& /*c2*/)
	{
		FT_NOT_IMPLEMENTED("ClampedAdd");
		return Color();
	}

	Color32		ClampedAdd(const Color32& /*c1*/, const Color32& /*c2*/)
	{
		FT_NOT_IMPLEMENTED("ClampedAdd");
		return Color32();
	}

	Color		ClampedSub(const Color& /*c1*/, const Color& /*c2*/)
	{
		FT_NOT_IMPLEMENTED("ClampedSub");
		return Color();
	}

	Color32		ClampedSub(const Color32& /*c1*/, const Color32& /*c2*/)
	{
		FT_NOT_IMPLEMENTED("ClampedSub");
		return Color32();
	}

	Color		ClampedMult(const Color& /*c1*/, float /*f*/)
	{
		FT_NOT_IMPLEMENTED("ClampedMult");
		return Color();
	}

	Color32		ClampedMult(const Color32& /*c1*/, float /*f*/)
	{
		FT_NOT_IMPLEMENTED("ClampedMult");
		return Color32();
	}
}
