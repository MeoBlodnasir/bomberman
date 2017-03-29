#include "InputHandler.hpp"
#include "Output.hpp"

InputHandler::InputHandler() {
  /*
  oBindings["Left"] = sf::Keyboard::Left; 
  oBindings["Right"] = sf::Keyboard::Right; 
  oBindings["Up"] = sf::Keyboard::Up; 
  oBindings["Down"] = sf::Keyboard::Down; 
  oBindings["Escape"] = sf::Keyboard::Escape; 
  */
}

void InputHandler::bind(const int key, const int action)
{
  if (oBindings.count(action) == 0)
    oBindings[action] = key;
}

void InputHandler::handleInput(sf::Event oEvent)
{
    if (oEvent.type == sf::Event::KeyPressed)
    {
      if (oKeysPressed[oEvent.key.code].isPressed == true)
        oKeysPressed[oEvent.key.code].changedThisFrame = false;
      else
        oKeysPressed[oEvent.key.code].changedThisFrame = true;
      oKeysPressed[oEvent.key.code].isPressed = true;
    }
    else if (oEvent.type == sf::Event::KeyReleased)
    {
      if (oKeysPressed[oEvent.key.code].isPressed == false)
        oKeysPressed[oEvent.key.code].changedThisFrame = false;
      else
        oKeysPressed[oEvent.key.code].changedThisFrame = true;
      oKeysPressed[oEvent.key.code].isPressed = false;
    }
}
InputHandler::KeyStatus InputHandler::isPressed(const int action)
{
  if (oBindings.count(action) > 0)
    return oKeysPressed[oBindings.at(action)];
  else
    return InputHandler::KeyStatus{false, false};
}

void    InputHandler::resetKeysPressedThisFrame()
{
  for (auto & x : oKeysPressed)
  {
    x.second.changedThisFrame = false;
  }

}
