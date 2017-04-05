#pragma once

#include <Core/Core.hpp>
#include <Core/Types.hpp>

namespace ft
{
	// fw
	class GridObject;

	class GridObjectController
	{
	public:

		GridObjectController(GridObject& oGridObject);
		~GridObjectController();

		void	Update(float32 fDt);

		void	SetMoveSpeed(float32 fMoveSpeed)						{ m_fMoveSpeed = fMoveSpeed; }

	protected:

		GridObject&		m_oPawn;

		float32			m_fMoveSpeed;

	private:

		GridObjectController();
		GridObjectController(const GridObjectController&) FT_DELETED;
		GridObjectController& operator = (const GridObjectController&) FT_DELETED;
	};
}
