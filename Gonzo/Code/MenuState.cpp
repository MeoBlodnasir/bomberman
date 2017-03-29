#include "MenuState.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "Game.hpp"
MenuState::MenuState(Game &game)
  : State(game)
{
 game.getInputHandler()->bind(sf::Keyboard::Up, InputHandler::E_MENU_UP); 
 game.getInputHandler()->bind(sf::Keyboard::Down, InputHandler::E_MENU_DOWN); 
 game.getInputHandler()->bind(sf::Keyboard::Right, InputHandler::E_MENU_RIGHT); 
 game.getInputHandler()->bind(sf::Keyboard::Left, InputHandler::E_MENU_LEFT); 
 game.getInputHandler()->bind(sf::Keyboard::Escape, InputHandler::E_MENU_QUIT); 
}
