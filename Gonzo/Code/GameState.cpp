#include "GameState.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "Game.hpp"
GameState::GameState(Game &game)
  : State(game)
{
 game.getInputHandler()->bind(sf::Keyboard::Up, InputHandler::E_MOVE_UP); 
 game.getInputHandler()->bind(sf::Keyboard::Down, InputHandler::E_MOVE_DOWN); 
 game.getInputHandler()->bind(sf::Keyboard::Right, InputHandler::E_MOVE_RIGHT); 
 game.getInputHandler()->bind(sf::Keyboard::Left, InputHandler::E_MOVE_LEFT); 
 game.getInputHandler()->bind(sf::Keyboard::Escape, InputHandler::E_ESCAPE); 
}
