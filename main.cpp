#include <vector>

#include "Game.h"
#include "Paddle.h"
#include "InputManager.h"
#include "Brick.h"
#include "Ball.h"
#include "Breakout.h"


auto createBricks(Breakout& game)
{
  const int horMarg = 50;
  const int verMarg = 50;
  const int space   = 20;
  const int ncols   = 10;
  const int nrows   = 7;
  const int w       = (1000 - 2*horMarg - space*ncols) / ncols;
  const int h       = 30;

  std::array<Brick, nrows * ncols> bs{};

  for (int i{}; i < nrows*ncols; i++) {
    int x = i % ncols;
    int y = i / ncols;
    bs[i] = Brick{w, h, horMarg + (float)x*(w+space), verMarg + (float)y*(h+space)};
    bs[i].setColor(i * 80, 90 + i * 50, 255, 255);
    game.addObject(&bs[i], false);
  }

  return bs;
}

int main ([[maybe_unused]]int argc, [[maybe_unused]]const char *argv[]) {
  InputManager im = InputManager
    ( SDLK_h      , MoveLeft
    , SDLK_a      , MoveLeft

    , SDLK_l      , MoveRight
    , SDLK_d      , MoveRight

    , SDLK_SPACE  , ReleaseBall
    , SDLK_f      , FireRocket

    , SDLK_ESCAPE , Quit
    );

  Game   g{ 1000, 1000, &im, 60 };
  Breakout b{};
  g.setScene(&b);

  Paddle p{};
  b.addObject(&p, true);

  auto bricks = createBricks(b);

  while (g.tick());
  g.dispose();
  return 0;
}
