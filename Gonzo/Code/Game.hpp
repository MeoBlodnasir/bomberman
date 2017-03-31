#include "InputHandler.hpp"
#include "MenuController.hpp"
#include "PlayerController.hpp"
class State;
#pragma once
class Game
{
  public:
    Game();
    void getInputs(sf::Event oEvent); 
    void changeState();
    InputHandler *getInputHandler();
    void    launchActions();
    void    resetKeysPressedThisFrame();
  protected:
    State *pCurrentState;
    InputHandler *pInputHandler;
    MenuController *pMenuController;
    PlayerController *pPlayerController;
};
