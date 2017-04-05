
#include "Grid.hpp"
#include "GridObject.hpp"
#include "PlayerController.hpp"

#include <Core/Output.hpp>
#include <Core/ProfilerBlock.hpp>
#include <Core/Path.hpp>
#include <Math/MathStructOutput.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <sstream>


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

		sf::Font oFont;
		sf::Text oText;
		oFont.loadFromFile(Path("./Djobi/Assets/Fonts/arial.ttf").GetFullPath());
		oText.setFont(oFont);
		oText.setFillColor(sf::Color::White);
		oText.setCharacterSize(20);

		Grid oGrid(21, 15, 30);
		{
			Player oPlayer(oGrid);
			PlayerController oController(oPlayer);
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
					oController.GetEvent(oEvent);
				}

				oController.Update(fDt);
				oGrid.Update(fDt);

				pWindow->clear();

				oGrid.Draw(*pWindow);

				std::stringstream oSS;
				oSS << "Player: " << oPlayer.GetPosition();
				oText.setString(oSS.str());
				pWindow->draw(oText);

				pWindow->display();
			}
		}
	}

	delete pWindow;

	return 0;
}
