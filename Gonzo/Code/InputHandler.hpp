#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>

class InputHandler
{
  public:
    enum EActions
    {
      E_MENU_UP = 0,
      E_MENU_DOWN,
      E_MENU_RIGHT,
      E_MENU_LEFT,
      E_MENU_QUIT, 
      E_MOVE_UP,
      E_MOVE_DOWN,
      E_MOVE_LEFT,
      E_MOVE_RIGHT,
      E_ESCAPE
    }; 
    InputHandler();
    void handleInput(sf::Event oEvent);
    void bind(const int key, const int action);
    bool isPressed(const int action);

  protected:
    std::map<int, bool> oKeysPressed;
    std::map<int, int> oBindings;
};
