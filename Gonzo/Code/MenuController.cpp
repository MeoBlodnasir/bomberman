#include "MenuController.hpp"
#include "InputHandler.hpp"
#include <Output.hpp>
#include "State.hpp"

int    menuUp() {
  FT_COUT << "Going up in menu" << std::endl; 
  return -1;
}
int    menuDown() {
  FT_COUT << "Going down in menu" << std::endl; 
  return -1;
}
int    menuLeft() {
  FT_COUT << "Going left in menu" << std::endl; 
  return -1;
}
int    menuRight() {
  FT_COUT << "Going right in menu" << std::endl; 
  return -1;
}
int    menuQuit() {
  FT_COUT << "quitting  menu" << std::endl; 
  return State::EState::E_GAME_STATE;
}

MenuController::MenuController()
{
  oActionMap[InputHandler::E_MENU_UP] = menuUp; 
  oActionMap[InputHandler::E_MENU_DOWN] = menuDown; 
  oActionMap[InputHandler::E_MENU_LEFT] = menuLeft; 
  oActionMap[InputHandler::E_MENU_RIGHT] = menuRight; 
  oActionMap[InputHandler::E_MENU_QUIT] = menuQuit; 
}

