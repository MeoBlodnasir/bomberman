
#include "PlayerController.hpp"

#include "GridObject.hpp"

#include <SFML\Window\Event.hpp>

#include <glm\detail\func_geometric.hpp>

namespace ft
{

	PlayerController::PlayerController(Player& oPlayer)
		: m_oPawn(oPlayer)
		, m_fMoveSpeed(1.f)
	{
	}

	PlayerController::~PlayerController()
	{
	}

	void	PlayerController::GetEvent(const sf::Event& oEvent)
	{
		if (oEvent.type == sf::Event::KeyPressed)
		{
			if (oEvent.key.code == sf::Keyboard::Space)
			{
				m_oPawn.DropBomb();
			}
		}
	}

	void PlayerController::Update(float32 fDt)
	{
		Vector2 vMovement(0.f, 0.f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			vMovement.x += -1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			vMovement.x += 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			vMovement.y += -1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			vMovement.y += 1;

		if (glm::dot(vMovement, vMovement) > 0.f)
			vMovement = glm::normalize(vMovement);
		vMovement *= fDt * m_fMoveSpeed;

		m_oPawn.Move(vMovement);
	}
}
