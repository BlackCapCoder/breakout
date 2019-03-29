#include <vector>

#include "Game.h"
#include "Breakout.h"


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

  Game g { 1000, 1000, &im, 60 };
  g.setScene<Breakout>();

  while (g.tick());
  g.dispose();
  return 0;
}
