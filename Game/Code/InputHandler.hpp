#include "Command.hpp"
#include <SFML/Window/Event.hpp>
#define MAX_ACTION_INDEX 4

class InputHandler
{
  public:
    InputHandler();
    void handleInput(sf::Event oEvent);
    void bind(int key, Command *command);

  private:
    std::array<Command*, MAX_ACTION_INDEX> commands;
    Command* upArrow;
    Command* downArrow;
    Command* leftArrow;
    Command* rightArrow;
    Command* escapeKey;
};
