#pragma once
class Game;
class State
{
  public:
  enum EState
  {
    E_GAME_STATE = 0,
    E_MENU_STATE
  };
    State(Game& game);
    int getState();
  protected:
    Game & oOwner;
    EState oState;
};
