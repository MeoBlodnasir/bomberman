
#include <Output.hpp>
#include <MathStructOutput.hpp>

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>

int		main()
{
	using namespace ft;

	ft::Matrix33 m33(1);
	
	FT_COUT << "Binvenu sur TESTS!" << std::endl;
	FT_COUT << m33 << std::endl;
	
	sf::ContextSettings oOpenGLContext;
	oOpenGLContext.depthBits		 = 24;
	oOpenGLContext.stencilBits		 = 8;
	oOpenGLContext.antialiasingLevel = 4;
	oOpenGLContext.majorVersion		 = 4;
	oOpenGLContext.minorVersion		 = 1;
	oOpenGLContext.attributeFlags	 = sf::ContextSettings::Core;
	sf::Window* pWindow = new sf::Window(sf::VideoMode(1080, 720), "Tests", sf::Style::Default, oOpenGLContext);
	pWindow->setFramerateLimit(60);

	bool bRunning = true;
	while (bRunning)
	{
		sf::Event oEvent;
		while (pWindow->pollEvent(oEvent))
		{
			if (oEvent.type == sf::Event::Closed || (oEvent.type == sf::Event::KeyPressed && oEvent.key.code == sf::Keyboard::Escape))
				bRunning = false;
		}
	}

	delete pWindow;

	return 0;
}
