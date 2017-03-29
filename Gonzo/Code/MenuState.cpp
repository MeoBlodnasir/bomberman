#include "MenuState.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "Game.hpp"
MenuState::MenuState(Game &game)
  : State(game)
{
 game.getInputHandler()->bind(sf::Keyboard::Up, "MenuUp"); 
 game.getInputHandler()->bind(sf::Keyboard::Down, "MenuDown"); 
 game.getInputHandler()->bind(sf::Keyboard::Right, "MenuRight"); 
 game.getInputHandler()->bind(sf::Keyboard::Left, "MenuLeft"); 
 game.getInputHandler()->bind(sf::Keyboard::Escape, "MenuQuit"); 
}
