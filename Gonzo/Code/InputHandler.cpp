#include "InputHandler.hpp"

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
      oKeysPressed[oEvent.key.code] = true;
    else if (oEvent.type == sf::Event::KeyReleased)
      oKeysPressed[oEvent.key.code] = false;
}
bool InputHandler::isPressed(const int action)
{
  if (oBindings.count(action) > 0)
    return oKeysPressed[oBindings.at(action)];
  else
    return false;
}
