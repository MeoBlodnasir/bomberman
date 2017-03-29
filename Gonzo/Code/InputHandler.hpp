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
    struct KeyStatus {
      bool isPressed;
      bool changedThisFrame;
    };
    InputHandler();
    void handleInput(sf::Event oEvent);
    void bind(const int key, const int action);
    KeyStatus isPressed(const int action);
    void    resetKeysPressedThisFrame();

  protected:
    std::map<int, KeyStatus> oKeysPressed;
    std::map<int, int> oBindings;
};
