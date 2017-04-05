
#include "Grid.hpp"
#include "GridObject.hpp"
#include "GridObjectController.hpp"

#include <Core/Output.hpp>
#include <Core/ProfilerBlock.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


int		TestsGameplay()
{
	using namespace ft;

	FT_COUT << "Bienvenu sur DJOBI!" << std::endl;

	sf::RenderWindow* pWindow = nullptr;
	{
		ProfilerBlockPrint oProfilerBlock("Fenetre SFML");
		pWindow = new sf::RenderWindow(sf::VideoMode(1080, 720), "Bomberman");
	}
	{
		pWindow->setFramerateLimit(60);

		sf::Clock oClock;
		oClock.restart();
		float fDt;

		Grid oGrid(21, 15, 30);
		Player oPlayer(oGrid);
		GridObjectController oController(oPlayer);
		oController.SetMoveSpeed(5.f);

		bool bRunning = true;
		while (bRunning)
		{
			sf::Time oDeltaTime = oClock.restart();
			fDt = oDeltaTime.asSeconds();

			sf::Event oEvent;
			while (pWindow->pollEvent(oEvent))
			{
				if (oEvent.type == sf::Event::Closed)
				{
					bRunning = false;
				}
				else if (oEvent.type == sf::Event::KeyPressed)
				{
					if (oEvent.key.code == sf::Keyboard::Escape)
					{
						bRunning = false;
					}
				}
			}

			oController.Update(fDt);

			pWindow->clear();

			oGrid.Draw(*pWindow);

			pWindow->display();
		}
	}

	delete pWindow;

	return 0;
}
