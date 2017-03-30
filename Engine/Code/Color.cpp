
#include "Color.hpp"

namespace ft
{
	Color::Color()
		: Vector4(0.f, 0.f, 0.f, 1.f)
	{
	}

	Color::Color(float fR, float fG, float fB, float fA /*= 1.f*/)
		: Vector4(fR, fG, fB, fA)
	{
	}

	Color::Color(const Color32& oColor32)
		: Vector4(oColor32.r / 255.f, oColor32.g / 255.f, oColor32.b / 255.f, oColor32.a / 255.f)
	{
	}

	Color::Color(const Color3& oColor3)
		: Vector4(oColor3)
	{
	}

	Color::operator Color3() const
	{
		return Color3(*this);
	}

	Color::operator Color32() const
	{
		return Color32(*this);
	}

	Color3::Color3()
		: Vector3(0.f, 0.f, 0.f)
	{
	}

	Color3::Color3(float fR, float fG, float fB)
		: Vector3(fR, fG, fB)
	{
	}

	Color3::Color3(const Color& oColor)
		: Vector3(oColor)
	{
	}

	Color3::Color3(const Color32& oColor32)
		: Vector3(oColor32.r / 255.f, oColor32.g / 255.f, oColor32.b / 255.f)
	{
	}

	Color3::operator Color() const
	{
		return Color(*this);
	}

	Color3::operator Color32() const
	{
		return Color32(*this);
	}

	Color32::Color32()
		: iColor(0)
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
		: iColor(iColor)
	{
	}

	Color32::Color32(const Color& oColor)
		: r((uint8)(oColor.r * 255.f))
		, g((uint8)(oColor.g * 255.f))
		, b((uint8)(oColor.b * 255.f))
		, a((uint8)(oColor.a * 255.f))
	{
	}

	Color32::Color32(const Color3& oColor3)
		: r((uint8)(oColor3.r * 255.f))
		, g((uint8)(oColor3.g * 255.f))
		, b((uint8)(oColor3.b * 255.f))
		, a((uint8)255)
	{
	}
}
