
#include "Collider.hpp"

#include <Math/Scalar.hpp>

#include <cmath>

namespace ft
{
	/*static*/
	bool	Collider::Collision(const CircleCollider& oCircle1, const CircleCollider& oCircle2, CollisionResult& oOutResult)
	{
		const float32 fDistanceX = oCircle1.m_vPosition.x - oCircle2.m_vPosition.x;
		const float32 fDistanceY = oCircle1.m_vPosition.y - oCircle2.m_vPosition.y;
		const float32 fDistanceSquared = fDistanceX * fDistanceX + fDistanceY * fDistanceY;
		const float32 fCircle1Radius = oCircle1.GetRadius();
		const float32 fCircle2Radius = oCircle2.GetRadius();
		const float32 fRadiusSumSquared = fCircle1Radius * fCircle1Radius + fCircle2Radius * fCircle2Radius;
		if (fDistanceSquared <= fRadiusSumSquared)
		{
			const float32 fDistance = ::sqrtf(fDistanceSquared);
			oOutResult.fPenetrationAmount = fDistance - ::sqrtf(fRadiusSumSquared);
			oOutResult.vPenetrationDir.x = fDistanceX / fDistance;
			oOutResult.vPenetrationDir.y = fDistanceY / fDistance;
			return true;
		}
		return false;
	}

	/*static*/
	bool	Collider::Collision(const RectangleCollider& oRect, const CircleCollider& oCircle, CollisionResult& oOutResult)
	{
		const float32 fClosestX = Clamp(oCircle.m_vPosition.x, oRect.GetLeft(), oRect.GetRight());
		const float32 fClosestY = Clamp(oCircle.m_vPosition.y, oRect.GetTop(), oRect.GetBottom());
		const float32 fDistanceX = oCircle.m_vPosition.x - fClosestX;
		const float32 fDistanceY = oCircle.m_vPosition.y - fClosestY;
		const float32 fDistanceSquared = fDistanceX * fDistanceX + fDistanceY * fDistanceY;
		const float32 fCircleRadius = oCircle.GetRadius();
		if (fDistanceSquared <= (fCircleRadius * fCircleRadius))
		{
			const float32 fDistance = ::sqrtf(fDistanceSquared);
			oOutResult.fPenetrationAmount = fDistance - fCircleRadius;
			oOutResult.vPenetrationDir.x = fDistanceX / fDistance;
			oOutResult.vPenetrationDir.y = fDistanceY / fDistance;
			return true;
		}
		return false;
	}

	/*static*/
	bool	Collider::Collision(const RectangleCollider& oRect1, const RectangleCollider& oRect2, CollisionResult& /*oOutResult*/)
	{
		const bool bIsOutRight = oRect1.GetRight() <= oRect2.GetLeft();
		const bool bIsOutLeft = oRect1.GetLeft() >= oRect2.GetRight();
		const bool bIsOutBottom = oRect1.GetBottom() <= oRect2.GetTop();
		const bool bIsOutTop = oRect1.GetTop() >= oRect2.GetBottom();
		return !(bIsOutBottom || bIsOutTop || bIsOutLeft || bIsOutRight);
		FT_TODO("oOutResult de Collision(RectangleCollider,RectangleCollider)");
	}


	RectangleCollider::RectangleCollider()
		: m_vSize(0.f, 0.f)
	{
	}

	RectangleCollider::RectangleCollider(const RectangleCollider& oCollider)
		: m_vSize(oCollider.m_vSize)
	{
	}

	RectangleCollider::~RectangleCollider()
	{
	}

	RectangleCollider&	RectangleCollider::operator = (const RectangleCollider& oCollider)
	{
		m_vPosition = oCollider.m_vPosition;
		m_vSize = oCollider.m_vSize;
		return *this;
	}

	bool	RectangleCollider::Collision(const RectangleCollider& oCollider, CollisionResult& oOutResult) const
	{
		return Collider::Collision(*this, oCollider, oOutResult);
	}

	bool	RectangleCollider::Collision(const CircleCollider& oCollider, CollisionResult& oOutResult) const
	{
		return Collider::Collision(*this, oCollider, oOutResult);
	}


	CircleCollider::CircleCollider()
		: m_fRadius(0.f)
	{
	}

	CircleCollider::CircleCollider(const CircleCollider& oCollider)
		: m_fRadius(oCollider.m_fRadius)
	{
	}

	CircleCollider::~CircleCollider()
	{
	}

	CircleCollider&		CircleCollider::operator = (const CircleCollider& oCollider)
	{
		m_vPosition = oCollider.m_vPosition;
		m_fRadius = oCollider.m_fRadius;
		return *this;
	}

	bool	CircleCollider::Collision(const RectangleCollider& oCollider, CollisionResult& oOutResult) const
	{
		const bool bResult = Collider::Collision(oCollider, *this, oOutResult);
		if (bResult)
			oOutResult.vPenetrationDir = -oOutResult.vPenetrationDir;
		return bResult;
	}

	bool	CircleCollider::Collision(const CircleCollider& oCollider, CollisionResult& oOutResult) const
	{
		return Collider::Collision(*this, oCollider, oOutResult);
	}
}
