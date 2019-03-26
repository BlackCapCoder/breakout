#include "Game.h"
#include "InputManager.h"

int keybinds[2 * 3] =
  { SDLK_h      , MoveLeft
  , SDLK_l      , MoveRight
  , SDLK_ESCAPE , Quit
  };

int main (int argc, const char *argv[]) {
  InputManager im = InputManager (keybinds, 3);

  Game g {1000, 1000, &im};
  // while (g.tick());
  g.dispose();
  return 0;
}
