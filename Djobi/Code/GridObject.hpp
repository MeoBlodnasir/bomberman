#pragma once

#include <Core/Core.hpp>
#include <Math/Vector2.hpp>

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

	class GridObject
	{
	public:

		GridObject(Grid& oGrid);
		virtual ~GridObject();

		void			SetPosition(const Vector2& vPos)					{ m_vPosition = vPos; }
		const Vector2&	GetPosition() const									{ return m_vPosition; }

		void			Move(const Vector2& vPos)							{ m_vPosition += vPos; }

		virtual void	Update(float32 fDt) = 0;
		virtual void	Draw(sf::RenderWindow&, const GridContext&) const = 0;

	protected:

		Grid&			m_oGrid;
		Vector2			m_vPosition;

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

		virtual void	Update(float32 fDt) override;
		virtual void	Draw(sf::RenderWindow& oRenderWindow, const GridContext& oGridContext) const override;

		void			DropBomb() const;

	protected:

		mutable sf::RectangleShape	m_oShape;
	};

	class Bomb : public GridObject
	{
	public:

		Bomb(Grid& oGrid);
		virtual ~Bomb();

		void			SetLifeTime(float32 fLifeTime)						{ m_fLifeTime = fLifeTime; }
		float32			GetLifeTime() const									{ return m_fLifeTime; }

		virtual void	Update(float32 fDt) override;
		virtual void	Draw(sf::RenderWindow& oRenderWindow, const GridContext& oGridContext) const override;

	protected:

		float32						m_fLifeTime;
		mutable sf::CircleShape		m_oShape;

		void			Explode();
	};
}
