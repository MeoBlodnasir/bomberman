
#include "Grid.hpp"

#include "GridObject.hpp"
#include <Utilities/SfmlConversions.hpp>

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
		Adjust();
	}
	Grid::~Grid()
	{
		FT_ASSERT(m_oObjects.size() == 0);
	}

	void	Grid::Draw(sf::RenderWindow& oRenderWindow) const
	{
		GridContext oGridContext;
		Vector2 vWindowSize = FromSFML(oRenderWindow.getSize());
		Vector2 vGridSize(m_iWidth * m_iCellSize, m_iHeight * m_iCellSize);

		oGridContext.iCellSize = m_iCellSize;
		oGridContext.vOrigin.x = (vWindowSize.x - vGridSize.x + oGridContext.iCellSize) / 2.f;
		oGridContext.vOrigin.y = (vWindowSize.y - vGridSize.y + oGridContext.iCellSize) / 2.f;

		for (uint32 y = 0; y < m_iHeight; ++y)
		{
			for (uint32 x = 0; x < m_iWidth; ++x)
			{
				m_oCellShape.setPosition(oGridContext.vOrigin.x + x * oGridContext.iCellSize, oGridContext.vOrigin.y + y * oGridContext.iCellSize);
				oRenderWindow.draw(m_oCellShape);
			}
		}

		for (const GridObject* pObject : m_oObjects)
			pObject->Draw(oRenderWindow, oGridContext);
	}

	void	Grid::Adjust()
	{
		m_oCellShape.setSize(sf::Vector2f((float32)m_iCellSize, (float32)m_iCellSize));
		m_oCellShape.setOrigin(m_oCellShape.getSize() / 2.f);
	}
}
