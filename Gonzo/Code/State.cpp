#include "Game.hpp"
#include "State.hpp"

State::State(Game& game)
  : oOwner(game)
{
}

int  State::getState()
{
  return oState;
}
