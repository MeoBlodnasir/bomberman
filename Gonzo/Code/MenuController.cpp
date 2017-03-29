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
  oActionMap[InputHandler::E_MENU_UP] = menuUp; 
  oActionMap[InputHandler::E_MENU_DOWN] = menuDown; 
  oActionMap[InputHandler::E_MENU_LEFT] = menuLeft; 
  oActionMap[InputHandler::E_MENU_RIGHT] = menuRight; 
  oActionMap[InputHandler::E_MENU_QUIT] = menuQuit; 
}

void    MenuController::checkActions(InputHandler *pInputHandler)
{
  for (auto const& x : oActionMap)
  {
    if (pInputHandler->isPressed(x.first).isPressed && pInputHandler->isPressed(x.first).changedThisFrame)
    {
      x.second();
    }
  }
}
