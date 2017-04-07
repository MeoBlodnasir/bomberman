#pragma once

#include <Core/Core.hpp>
#include <Math/Vector2.hpp>

#include "Transformable.hpp"

namespace ft
{
	// fw
	class RectangleCollider;
	class CircleCollider;

	struct CollisionResult
	{
		Vector2	vPenetrationDir;
		float32	fPenetrationAmount;
	};

	class Collider : public Transformable
	{
	public:

		Collider()															{}
		Collider(const Collider&)											{}
		virtual ~Collider()													{}

		Collider&		operator = (const Collider&)						{}

		virtual bool	Collision(const RectangleCollider&, CollisionResult&) const = 0;
		virtual bool	Collision(const CircleCollider&, CollisionResult&) const = 0;

		static bool		Collision(const CircleCollider&, const CircleCollider&, CollisionResult&);
		static bool		Collision(const RectangleCollider&, const CircleCollider&, CollisionResult&);
		static bool		Collision(const RectangleCollider&, const RectangleCollider&, CollisionResult&);
	};

	class RectangleCollider : public Collider
	{
	public:

		RectangleCollider();
		RectangleCollider(const RectangleCollider& oCollider);
		virtual ~RectangleCollider();

		RectangleCollider&		operator = (const RectangleCollider& oCollider);

		inline void				SetSize(const Vector2& vSize)					{ m_vSize = vSize; }
		inline const Vector2&	GetSize() const									{ return m_vSize; }

		inline float32			GetLeft() const									{ return m_vPosition.x - m_vSize.x * 0.5f; }
		inline float32			GetRight() const								{ return m_vPosition.x + m_vSize.x * 0.5f; }
		inline float32			GetTop() const									{ return m_vPosition.y - m_vSize.y * 0.5f; }
		inline float32			GetBottom() const								{ return m_vPosition.y + m_vSize.y * 0.5f; }

		virtual bool			Collision(const RectangleCollider& oCollider, CollisionResult& oOutResult) const override;
		virtual bool			Collision(const CircleCollider& oCollider, CollisionResult& oOutResult) const override;

	protected:

		Vector2		m_vSize;
	};

	class CircleCollider : public Collider
	{
	public:

		CircleCollider();
		CircleCollider(const CircleCollider& oCollider);
		virtual ~CircleCollider();

		CircleCollider&		operator = (const CircleCollider& oCollider);

		inline void			SetRadius(float32 fRadius)							{ m_fRadius = fRadius; }
		inline float32		GetRadius() const									{ return m_fRadius; }

		virtual bool		Collision(const RectangleCollider& oCollider, CollisionResult& oOutResult) const override;
		virtual bool		Collision(const CircleCollider& oCollider, CollisionResult& oOutResult) const override;

	protected:

		float32		m_fRadius;
	};
}
