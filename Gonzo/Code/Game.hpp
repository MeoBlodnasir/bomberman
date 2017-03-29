#include "InputHandler.hpp"
class State;
class MenuState;
class Game
{
  public:
    Game();
    void getInputs(sf::Event oEvent); 
    void changeState();
    InputHandler *getInputHandler();
  protected:
    State *pCurrentState;
    InputHandler *pInputHandler;
};
