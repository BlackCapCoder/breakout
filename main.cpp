#include <vector>

#include "Game.h"
#include "Paddle.h"
#include "InputManager.h"
#include "Brick.h"
#include "Ball.h"
#include "Text.h"


auto createBricks(Game& game)
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
    game.addObject(&bs[i]);
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

    , SDLK_ESCAPE , Quit
    );

  Game   g{ 1000, 1000, &im, 60 };
  Paddle p{};
  Ball   b{};

  Text text{g.getRender(), "Hello", SDL_Color{255,255,255,0}, 24, 100, 400};

  g.addObject(&p);
  g.addObject(&b);

  auto bricks = createBricks(g);

  g.addObject(&text);

  while (g.tick());
  g.dispose();
  return 0;
}
