#include "MenuState.hpp"
#include "GameState.hpp"
#include "Game.hpp"

Game::Game()
{
  pInputHandler = new InputHandler();
  pCurrentState = new MenuState(*this);
  pMenuController = new MenuController();
  
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

void    Game::launchActions()
{
  switch (pCurrentState->getState())
  {

  }
  switch (pMenuController->checkActions(pInputHandler))
  {
    case -1:
      return;
    case State::EState::E_GAME_STATE:
      delete(pCurrentState);
      pCurrentState = new GameState(*this);
      break ;
    case State::EState::E_MENU_STATE:
      delete(pCurrentState);
      pCurrentState = new MenuState(*this);
      break ;
    default:
      return;
  }
}
void    Game::resetKeysPressedThisFrame()
{
  pInputHandler->resetKeysPressedThisFrame();
}

