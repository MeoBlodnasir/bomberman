#pragma once
#include <map>
class InputHandler;
class Controller
{
  public:
    Controller();
    int   checkActions(InputHandler *pInputHandler);
  protected:
    std::map<int, int (*)()> oActionMap;
};
