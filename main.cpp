#include "Game.h"
#include "Paddle.h"
#include "InputManager.h"

#define NUM_KEYBINDS 3

int keybinds[2 * NUM_KEYBINDS] =
  { SDLK_h      , MoveLeft
  , SDLK_l      , MoveRight
  , SDLK_ESCAPE , Quit
  };

Paddle p{};

int main (int argc, const char *argv[]) {
  InputManager im = InputManager (keybinds, NUM_KEYBINDS);

  Game g {1000, 1000, &im};

  g.addObject(&p);

  while (g.tick());
  g.dispose();
  return 0;
}
