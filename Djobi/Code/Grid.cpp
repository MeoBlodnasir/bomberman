
#include "Grid.hpp"

#include "GridObject.hpp"

#include <SFML\Graphics\RenderWindow.hpp>

namespace ft
{
	Grid::Grid(uint32 iWidth, uint32 iHeight, uint32 iCellSize)
		: m_iWidth(iWidth)
		, m_iHeight(iHeight)
		, m_iCellSize(iCellSize)
	{
		m_oCellShape.setFillColor(sf::Color(25, 25, 25, 255));
		m_oCellShape.setOutlineThickness(1.f);
		m_oCellShape.setOutlineColor(sf::Color(50, 50, 50, 255));
		SetCellSize(iCellSize);
	}

	Grid::~Grid()
	{
	}

	void	Grid::SetCellSize(uint32 iCellSize)
	{
		m_iCellSize = iCellSize;
		m_oGridContext.iCellSize = m_iCellSize;
		AdjustCellShape();
	}

	void	Grid::SetWindowSize(const Vector2& vWindowSize)
	{
		m_oGridContext.vOrigin.x = (vWindowSize.x - (m_iWidth * m_iCellSize) + m_iCellSize) * 0.5f;
		m_oGridContext.vOrigin.y = (vWindowSize.y - (m_iHeight * m_iCellSize) + m_iCellSize) * 0.5f;
	}

	void	Grid::Update(float32 /*fDt*/)
	{
	}

	void	Grid::Draw(sf::RenderWindow& oRenderWindow) const
	{
		for (uint32 y = 0; y < m_iHeight; ++y)
		{
			for (uint32 x = 0; x < m_iWidth; ++x)
			{
				m_oCellShape.setPosition(m_oGridContext.vOrigin.x + x * m_iCellSize, m_oGridContext.vOrigin.y + y * m_iCellSize);
				oRenderWindow.draw(m_oCellShape);
			}
		}
	}

	void	Grid::AdjustCellShape()
	{
		m_oCellShape.setSize(sf::Vector2f((float32)m_iCellSize, (float32)m_iCellSize));
		m_oCellShape.setOrigin(m_oCellShape.getSize() * 0.5f);
	}
}
