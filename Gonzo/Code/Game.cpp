#include "MenuState.hpp"
#include "Game.hpp"

Game::Game()
{
  pCurrentState = new MenuState(*this);
}

void    Game::getInputs(sf::Event oEvent)
{
  pInputHandler->handleInput(oEvent);
}
void    Game::changeState() {

}
