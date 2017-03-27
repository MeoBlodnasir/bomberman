
void goUp() {
}
void goDown() {
}
void goLeft() {
}
void goRight() {
}
void quit() {
}

class MoveUpCommand : public Command{
  public:
    virtual void execute() { goUp(); }
};
class MoveDownCommand : public Command
{
  public:
    virtual void execute() { goDown(); }
};
class MoveLeftCommand : public Command{
  public:
    virtual void execute() { goLeft(); }
};
class MoveRightCommand : public Command
{
  public:
    virtual void execute() { goRight(); }
};
class QuitCommand : public Command
{
  public:
    virtual void execute() { quit(); }
};
void InputHandler::bind(int key, Command *command)
{
      commands[key] = command;   
}


