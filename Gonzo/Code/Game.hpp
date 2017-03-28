#include "InputHandler.hpp"
class State;
class MenuState;
class Game
{
  public:
    Game();
    void getInputs(sf::Event oEvent); 
    void changeState();
  private:
    State *pCurrentState;
    InputHandler *pInputHandler;
};
