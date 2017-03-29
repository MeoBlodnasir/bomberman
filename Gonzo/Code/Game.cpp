#include "MenuState.hpp"
#include "Game.hpp"

Game::Game()
{
  pInputHandler = new InputHandler();
  pCurrentState = new MenuState(*this);
}

void    Game::getInputs(sf::Event oEvent)
{
  pInputHandler->handleInput(oEvent);
}
void    Game::changeState() {

}

InputHandler *Game::getInputHandler()
{
  return pInputHandler;
}
