#include "InputHandler.hpp"
#include "Actions.hpp"
enum ECommand
{
  E_UP = 0,
  E_DOWN,
  E_LEFT,
  E_RIGHT,
  E_ESC
};

InputHandler::InputHandler() {
  upArrow = new MoveUpCommand();
  downArrow = new MoveDownCommand();
  leftArrow = new MoveLeftCommand();
  rightArrow = new MoveRightCommand();
  escapeKey = new QuitCommand();
  commands[E_UP]       = upArrow;
  commands[E_LEFT]     = leftArrow;
  commands[E_DOWN]     = rightArrow;
  commands[E_RIGHT]    = downArrow;
  commands[E_ESC]    = escapeKey;
}

void InputHandler::handleInput(sf::Event oEvent)
{
  if (oEvent.type == sf::Event::Closed || (oEvent.type == sf::Event::KeyPressed && oEvent.key.code == sf::Keyboard::Escape))
    escapeKey->execute();
}
