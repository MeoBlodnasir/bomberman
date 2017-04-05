#pragma once

#include <Core/Core.hpp>
#include <Math/Vector2.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

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

		virtual void	Draw(sf::RenderWindow&, const GridContext&) const = 0;

	protected:

		Grid&			m_oOwner;
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

		virtual void	Draw(sf::RenderWindow& oRenderWindow, const GridContext& oGridContext) const;

	protected:

		mutable sf::RectangleShape	m_oShape;
	};
}
