#include "GameState.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "Game.hpp"
GameState::GameState(Game &game)
  : State(game)
{
 game.getInputHandler()->bind(sf::Keyboard::Up, "MoveUp"); 
 game.getInputHandler()->bind(sf::Keyboard::Down, "MoveDown"); 
 game.getInputHandler()->bind(sf::Keyboard::Right, "MoveRight"); 
 game.getInputHandler()->bind(sf::Keyboard::Left, "MoveLeft"); 
 game.getInputHandler()->bind(sf::Keyboard::Escape, "GameQuit"); 
}
