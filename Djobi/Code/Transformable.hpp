#pragma once

#include <Math/Vector2.hpp>

namespace ft
{
	class Transformable
	{
	public:

		Transformable();
		Transformable(const Transformable& oTransformable);
		virtual ~Transformable();

		Transformable&	operator = (const Transformable& oTransformable);

		void			SetPosition(const Vector2& vPos)					{ m_vPosition = vPos; }
		const Vector2&	GetPosition() const									{ return m_vPosition; }

		void			Move(const Vector2& vPos)							{ m_vPosition += vPos; }

	protected:

		Vector2			m_vPosition;
	};
}
