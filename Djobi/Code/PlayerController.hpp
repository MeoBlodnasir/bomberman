#pragma once

#include <Core/Core.hpp>
#include <Core/Types.hpp>

// fw
namespace sf
{
	class Event;
}

namespace ft
{
	// fw
	class Player;

	class PlayerController
	{
	public:

		PlayerController(Player& oPlayer);
		~PlayerController();

		void	GetEvent(const sf::Event& oEvent);

		void	Update(float32 fDt);

		void	SetMoveSpeed(float32 fMoveSpeed)						{ m_fMoveSpeed = fMoveSpeed; }

	protected:

		Player&		m_oPawn;
		float32		m_fMoveSpeed;

	private:

		PlayerController();
		PlayerController(const PlayerController&) FT_DELETED;
		PlayerController& operator = (const PlayerController&) FT_DELETED;
	};
}
