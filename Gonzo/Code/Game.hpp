#include "InputHandler.hpp"
#include "MenuController.hpp"
class State;
class MenuState;
class Game
{
  public:
    Game();
    void getInputs(sf::Event oEvent); 
    void changeState();
    InputHandler *getInputHandler();
    void    launchControllerActions();
  protected:
    State *pCurrentState;
    InputHandler *pInputHandler;
    MenuController *pMenuController;
};
