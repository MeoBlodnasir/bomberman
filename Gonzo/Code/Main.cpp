
#if defined(_WIN32)
#	include <crtdbg.h>
#endif

#include <Output.hpp>
#include <OpenGL.hpp>
#include <ProfilerBlock.hpp>
#include <Mesh.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>
#include <Camera.hpp>
#include <Quaternion.hpp>
#include "Game.hpp"
#include "MenuController.hpp"

#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>


int		main()
{
#if defined(_WIN32)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif


  FT_COUT << "Binvenu sur GONZO!" << std::endl;
  // create the window
  sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
  sf::CircleShape shape(50);
  shape.setFillColor(sf::Color(100, 250, 50));
  Game game; 
  MenuController menuController;


  // run the program as long as the window is open
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (window.pollEvent(event))
    {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed)
        window.close();
      game.getInputs(event);
      menuController.checkActions(game.getInputHandler());
    }

    // clear the window with black color
    window.clear(sf::Color::Black);

    // draw everything here...
    // window.draw(...);
    window.draw(shape);

    // end the current frame
    window.display();
  }


  return 0;
}
