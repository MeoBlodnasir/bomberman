
#include "GridObjectController.hpp"

#include "GridObject.hpp"

#include <SFML\Window\Keyboard.hpp>

#include <glm\detail\func_geometric.hpp>

namespace ft
{

	GridObjectController::GridObjectController(GridObject& oGridObject)
		: m_oPawn(oGridObject)
		, m_fMoveSpeed(1.f)
	{
	}

	GridObjectController::~GridObjectController()
	{
	}

	void GridObjectController::Update(float32 fDt)
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
