#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>

class InputHandler
{
  public:
    InputHandler();
    void handleInput(sf::Event oEvent);
    void bind(const int key, const std::string& action);
    bool isPressed(const std::string& action);

  protected:
      std::map<int, bool> oKeysPressed;
      std::map<std::string, int> oBindings;
};
