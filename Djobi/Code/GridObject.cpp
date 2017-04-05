
#include "GridObject.hpp"

#include "Grid.hpp"

#include <SFML\Graphics\RenderWindow.hpp>

namespace ft
{
	GridObject::GridObject(Grid& oGrid)
		: m_oOwner(oGrid)
		, m_vPosition(0.f, 0.f)
	{
		m_oOwner.RegisterGridObject(this);
	}

	GridObject::~GridObject()
	{
		m_oOwner.UnregisterGridObject(this);
	}


	Player::Player(Grid& oGrid)
		: GridObject(oGrid)
	{
		m_oShape.setFillColor(sf::Color(200, 160, 50, 255));
		m_oShape.setOutlineThickness(1.f);
		m_oShape.setOutlineColor(sf::Color(255, 200, 100, 255));
	}

	Player::~Player()
	{
	}

	void	Player::Draw(sf::RenderWindow& oRenderWindow, const GridContext& oGridContext) const
	{
		m_oShape.setSize(sf::Vector2f((float32)(oGridContext.iCellSize - 5), (float32)(oGridContext.iCellSize - 5)));
		m_oShape.setOrigin(m_oShape.getSize() / 2.f);
		m_oShape.setPosition(m_vPosition.x * oGridContext.iCellSize + oGridContext.vOrigin.x, m_vPosition.y * oGridContext.iCellSize + oGridContext.vOrigin.y);
		oRenderWindow.draw(m_oShape);
	}
}
