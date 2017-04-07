
#include "GridObject.hpp"

#include "Grid.hpp"

#include <SFML\Graphics\RenderWindow.hpp>

#include <sstream>

namespace ft
{
	GridObject::GridObject(Grid& oGrid)
		: m_oGrid(oGrid)
	{
	}

	GridObject::~GridObject()
	{
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

	void	Player::Update(float32 /*fDt*/)
	{
	}

	void	Player::Draw(sf::RenderWindow& oRenderWindow, const GridContext& oGridContext) const
	{
		m_oShape.setSize(sf::Vector2f(oGridContext.iCellSize - 5.f, oGridContext.iCellSize - 5.f));
		m_oShape.setOrigin(m_oShape.getSize() * 0.5f);
		m_oShape.setPosition(m_vPosition.x * oGridContext.iCellSize + oGridContext.vOrigin.x, m_vPosition.y * oGridContext.iCellSize + oGridContext.vOrigin.y);
		oRenderWindow.draw(m_oShape);
	}

	void	Player::DropBomb() const
	{
		Bomb* pBomb = new Bomb(m_oGrid);
		pBomb->SetPosition(Vector2(::ceilf(m_vPosition.x - 0.5f), ::ceilf(m_vPosition.y - 0.5f))); // au centre d'une case, toujours
		pBomb->SetLifeTime(2.f);
	}


	Bomb::Bomb(Grid& oGrid)
		: GridObject(oGrid)
		, m_fLifeTime(100000.f)
	{
		m_oShape.setFillColor(sf::Color(40, 0, 100, 255));
		m_oShape.setOutlineThickness(1.f);
		m_oShape.setOutlineColor(sf::Color(50, 75, 100, 255));
	}

	Bomb::~Bomb()
	{
	}

	void	Bomb::Update(float32 fDt)
	{
		m_fLifeTime -= fDt;
		if (m_fLifeTime < 0.f)
			Explode();
	}

	void	Bomb::Draw(sf::RenderWindow& oRenderWindow, const GridContext& oGridContext) const
	{
		m_oShape.setRadius((oGridContext.iCellSize - 5.f) * 0.5f);
		m_oShape.setOrigin(m_oShape.getRadius(), m_oShape.getRadius());
		m_oShape.setPosition(m_vPosition.x * oGridContext.iCellSize + oGridContext.vOrigin.x, m_vPosition.y * oGridContext.iCellSize + oGridContext.vOrigin.y);
		oRenderWindow.draw(m_oShape);
	}

	void	Bomb::Explode()
	{
		delete this;
	}
}
