#include <vector>
#include <algorithm>

#include "Game.h"
#include "Paddle.h"
#include "InputManager.h"
#include "Brick.h"
#include "Ball.h"

#define NUM_KEYBINDS 3

int keybinds[2 * NUM_KEYBINDS] =
  { SDLK_h      , MoveLeft
  , SDLK_l      , MoveRight
  , SDLK_ESCAPE , Quit
  };

int main ([[maybe_unused]]int argc, [[maybe_unused]]const char *argv[]) {
  InputManager im = InputManager (keybinds, NUM_KEYBINDS);

  Game   g{ 1000, 1000, &im };
  Paddle p{};
  Ball   b{};

  g.addObject(&p);
  g.addObject(&b);

  const int horMarg = 50;
  const int verMarg = 50;
  const int space = 20;
  const int ncols = 10;
  const int nrows = 7;

  const int w = (1000 - 2*horMarg - space*ncols) / ncols;
  const int h = 30;

  std::vector<Brick> bs(nrows*ncols);
  // Brick bs[nrows*ncols];

  std::generate(std::begin(bs), std::end(bs), [=, &g, i = 0] () {
    int x = i % ncols;
    int y = i / ncols;
    Brick b{w, h, horMarg + (float)x * (w + space), verMarg + (float)y * (h + space)};
    b.setColor(i * 80, 90 + i * 50, 255, 255);
    return b;
  });

  for (auto& b : bs) g.addObject(&b);

  // for (int i = 0; i < nrows*ncols; i++) {
  //   int x = i % ncols, y = i / ncols;
  //   bs[i] = Brick{w, h, horMarg + (float)x*(w+space), verMarg + (float)y*(h+space)};
  //   bs[i].setColor(i * 80, 90 + i * 50, 255, 255);
  //   g.addObject(&bs[i]);
  // }

  while (g.tick());
  g.dispose();
  return 0;
}
