#include <vector>

#include "Game.h"
#include "Paddle.h"
#include "InputManager.h"
#include "Brick.h"
#include "Ball.h"
#include "Text.h"
#include "Breakout.h"
#include "MainMenu.h"

int main ([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[])
{
  InputManager im = InputManager
    ( SDLK_h      , MoveLeft
    , SDLK_a      , MoveLeft

    , SDLK_l      , MoveRight
    , SDLK_d      , MoveRight

    , SDLK_j      , MoveDown
    , SDLK_s      , MoveDown

    , SDLK_k      , MoveUp
    , SDLK_w      , MoveUp

    , SDLK_SPACE  , ReleaseBall
    , SDLK_f      , FireRocket

    , SDLK_ESCAPE , Quit
    );

  Game g { 1000, 1000, &im, 60 };
  g.setScene<MainMenu>();

  while (g.tick());
  g.dispose();

  return 0;
}
