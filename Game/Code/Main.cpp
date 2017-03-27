
#if defined(_WIN32)
#	include <crtdbg.h>
#endif

#include <Output.hpp>
#include <OpenGL.hpp>
#include "InputHandler.hpp"


#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>

int		main()
{
#if defined(_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	FT_COUT << "Binvenu sur GAME!" << std::endl;

	sf::ContextSettings oOpenGLContext;
	oOpenGLContext.depthBits		 = 24;
	oOpenGLContext.stencilBits		 = 8;
	oOpenGLContext.antialiasingLevel = 4;
	oOpenGLContext.majorVersion		 = 4;
	oOpenGLContext.minorVersion		 = 1;
	oOpenGLContext.attributeFlags	 = sf::ContextSettings::Core;
	sf::Window* pWindow = new sf::Window(sf::VideoMode(1080, 720), "Bomberman", sf::Style::Default, oOpenGLContext);
	pWindow->setFramerateLimit(60);

#if defined(_WIN32)
	// Après l'initialisation d'un contexte OpenGL
	{
		glewExperimental = GL_TRUE; // core profile
		const GLenum eGlewError = glewInit();
		if (eGlewError != GLEW_OK)
		{
			FT_CERR << "Erreur dans l'initialisation de GLEW: " << glewGetErrorString(eGlewError) << std::endl;
			return -1;
		}
	}
#endif

	glViewport(0, 0, (GLsizei)pWindow->getSize().x, (GLsizei)pWindow->getSize().y);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
	
	bool bRunning = true;
    InputHandler inputHandler;
	while (bRunning)
	{
		sf::Event oEvent;
		while (pWindow->pollEvent(oEvent))
		{
          inputHandler.handleInput(oEvent);
		}
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		pWindow->display();
	}

	delete pWindow;

	return 0;
}
