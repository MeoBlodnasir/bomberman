#pragma once

#include "Collider.hpp"

#include <Core/Core.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

// fw
namespace sf
{
	class RenderWindow;
}

namespace ft
{
	// fw
	class Grid;
	struct GridContext;

	class GridObject : public Transformable
	{
	public:

		GridObject(Grid& oGrid);
		virtual ~GridObject();

		virtual void				Update(float32 fDt) = 0;
		virtual void				Draw(sf::RenderWindow&, const GridContext&) const = 0;
		virtual const Collider*		GetCollider() const	= 0;

	protected:

		Grid&						m_oGrid;

	private:

		GridObject();
		GridObject(const GridObject&) FT_DELETED;
		GridObject& operator = (const GridObject&) FT_DELETED;
	};

	class Player : public GridObject
	{
	public:

		Player(Grid& oGrid);
		virtual ~Player();

		virtual void				Update(float32 fDt) override;
		virtual void				Draw(sf::RenderWindow& oRenderWindow, const GridContext& oGridContext) const override;
		virtual const Collider*		GetCollider() const override			{ return &m_oCollider; }

		void						DropBomb() const;

	protected:

		CircleCollider				m_oCollider;
		mutable sf::RectangleShape	m_oShape;
	};

	class Bomb : public GridObject
	{
	public:

		Bomb(Grid& oGrid);
		virtual ~Bomb();

		void						SetLifeTime(float32 fLifeTime)			{ m_fLifeTime = fLifeTime; }
		float32						GetLifeTime() const						{ return m_fLifeTime; }

		virtual void				Update(float32 fDt) override;
		virtual void				Draw(sf::RenderWindow& oRenderWindow, const GridContext& oGridContext) const override;
		virtual const Collider*		GetCollider() const override			{ return &m_oCollider; }

	protected:

		float32						m_fLifeTime;
		RectangleCollider			m_oCollider;
		mutable sf::CircleShape		m_oShape;

		void						Explode();
	};
}
