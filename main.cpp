#include "Game.h"
#include "Paddle.h"
#include "InputManager.h"
#include "Brick.h"
#include "Ball.h"

#include <vector>


#define NUM_KEYBINDS 3

int keybinds[2 * NUM_KEYBINDS] =
  { SDLK_h      , MoveLeft
  , SDLK_l      , MoveRight
  , SDLK_ESCAPE , Quit
  };

Paddle p{};
Ball   b{};


int main (int argc, const char *argv[]) {
  InputManager im = InputManager (keybinds, NUM_KEYBINDS);

  Game g {1000, 1000, &im};

  g.addObject(&p);
  g.addObject(&b);

  Brick bs[9*5];

  for (int i = 0; i < 9*5; i++) {
    int x = i % 9, y = i / 9;
    bs[i] = Brick{98, 30, 20 + (float)x*(98+10), 20 + (float)y*(30+10)};
    bs[i].setColor(i * 80, 90 + i * 50, 255, 255);
    g.addObject(&bs[i]);
  }

  while (g.tick());
  g.dispose();
  return 0;
}
