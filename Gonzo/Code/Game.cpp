#include "MenuState.hpp"
#include "GameState.hpp"
#include "Game.hpp"

Game::Game()
{
  pInputHandler = new InputHandler();
  pCurrentState = new MenuState(*this);
  pMenuController = new MenuController();
  pPlayerController = new PlayerController();
  
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
  int newState; 
  switch (pCurrentState->getState())
  {
    case State::EState::E_GAME_STATE:
      newState = pPlayerController->checkActions(pInputHandler);
      break ;
    case State::EState::E_MENU_STATE:
      newState = pMenuController->checkActions(pInputHandler);
      break;
    default:
      newState = -1;
  }
  switch (newState)
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

