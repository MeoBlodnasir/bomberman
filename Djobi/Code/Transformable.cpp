
#include "Transformable.hpp"

namespace ft
{
	Transformable::Transformable()
		: m_vPosition(0.f, 0.f)
	{
	}

	Transformable::Transformable(const Transformable& oTransformable)
		: m_vPosition(oTransformable.m_vPosition)
	{
	}

	Transformable::~Transformable()
	{
	}

	Transformable& Transformable::operator = (const Transformable& oTransformable)
	{
		m_vPosition = oTransformable.m_vPosition;
		return *this;
	}
}
