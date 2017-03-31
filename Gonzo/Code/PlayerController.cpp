#include "PlayerController.hpp"
#include "InputHandler.hpp"
#include <Output.hpp>
#include "State.hpp"

int    moveUp() {
  FT_COUT << "player moving up" << std::endl; 
  return -1;
}
int    moveDown() {
  FT_COUT << "player moving down" << std::endl; 
  return -1;
}
int    moveLeft() {
  FT_COUT << "player moving left" << std::endl; 
  return -1;
}
int    moveRight() {
  FT_COUT << "player moving right" << std::endl; 
  return -1;
}
int    gameQuit() {
  FT_COUT << "player quitting game" << std::endl; 
  return State::EState::E_MENU_STATE;
}

PlayerController::PlayerController()
{
  oActionMap[InputHandler::E_MOVE_UP] = moveUp; 
  oActionMap[InputHandler::E_MOVE_DOWN] = moveDown; 
  oActionMap[InputHandler::E_MOVE_LEFT] = moveLeft; 
  oActionMap[InputHandler::E_MOVE_RIGHT] = moveRight; 
  oActionMap[InputHandler::E_ESCAPE] = gameQuit; 
}
