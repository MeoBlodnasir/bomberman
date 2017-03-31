#include "Controller.hpp"
#include "InputHandler.hpp"

Controller::Controller()
{
}


int    Controller::checkActions(InputHandler *pInputHandler)
{
  int nextState = -1;
  int tmpState;
  for (auto const& x : oActionMap)
  {
    if (pInputHandler->isPressed(x.first).isPressed && pInputHandler->isPressed(x.first).changedThisFrame)
    {
      tmpState = x.second();
      if (tmpState != -1)
        nextState = tmpState;
    }
  }
  return nextState;
}
