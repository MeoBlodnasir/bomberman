#include "MenuController.hpp"
#include "InputHandler.hpp"
#include <Output.hpp>

void    menuUp() {
  FT_COUT << "Going up in menu" << std::endl; 
}
void    menuDown() {
  FT_COUT << "Going down in menu" << std::endl; 
}
void    menuLeft() {
  FT_COUT << "Going left in menu" << std::endl; 
}
void    menuRight() {
  FT_COUT << "Going right in menu" << std::endl; 
}
void    menuQuit() {
  FT_COUT << "quitting  menu" << std::endl; 
}

MenuController::MenuController()
{
  oActionMap["MenuUp"] = menuUp; 
  oActionMap["MenuDown"] = menuDown; 
  oActionMap["MenuLeft"] = menuLeft; 
  oActionMap["MenuRight"] = menuRight; 
  oActionMap["MenuQuit"] = menuQuit; 
}

void    MenuController::checkActions(InputHandler *pInputHandler)
{
  for (auto const& x : oActionMap)
  {
    if (pInputHandler->isPressed(x.first))
    {
      x.second();
    }
  }
}
