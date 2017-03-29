#include <map>
class InputHandler;
class MenuController
{
  public:
    MenuController();
    void   checkActions(InputHandler *pInputHandler);
  private:
      std::map<std::string, void (*)()> oActionMap;
};
