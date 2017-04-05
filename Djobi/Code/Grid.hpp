#pragma once

#include <Core/Core.hpp>
#include <Math/Vector2.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

#include <set>

// fw
namespace sf
{
	class RenderWindow;
}

namespace ft
{
	// fw
	class GridObject;

	struct GridContext
	{
		uint32	iCellSize;
		Vector2 vOrigin;
	};

	class Grid
	{
	public:

		Grid(uint32 iWidth, uint32 iHeight, uint32 iCellSize);
		~Grid();

		void	SetGridDimentions(uint32 iWidth, uint32 iHeight)			{ m_iWidth = iWidth; m_iHeight = iHeight; }
		void	SetCellSize(uint32 iCellSize)								{ m_iCellSize = iCellSize; Adjust(); }

		uint32	GetHeight() const											{ return m_iHeight; }
		uint32	GetWidth() const											{ return m_iWidth; }
		uint32	GetCellSize() const											{ return m_iCellSize; }

		void	RegisterGridObject(GridObject* pObject)						{ m_oObjects.insert(pObject); }
		void	UnregisterGridObject(GridObject* pObject)					{ m_oObjects.erase(pObject); }

		void	Update(float32 fDt);
		void	Draw(sf::RenderWindow& oRenderWindow) const;

	private:

		uint32						m_iWidth;		// en nombre de cellules
		uint32						m_iHeight;		// en nombre de cellules
		uint32						m_iCellSize;	// en pixels

		mutable sf::RectangleShape	m_oCellShape;

		std::set<GridObject*>		m_oObjects;

		void	Adjust();

	private:

		Grid();
		Grid(const Grid&) FT_DELETED;
		Grid& operator = (const Grid&) FT_DELETED;
	};
}
