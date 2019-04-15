#include <iostream>

#include "Game.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Breakout.h"
#include "MainMenu.h"
#include "Highscore.h"


int main (void)
{
  using kbs = Keybinds
    < SDLK_h      , MoveLeft
    , SDLK_l      , MoveRight
    , SDLK_j      , MoveDown
    , SDLK_k      , MoveUp

    , SDLK_SPACE  , ReleaseBall
    , SDLK_f      , FireRocket
    , SDLK_d      , PowerMagnet

    , SDLK_ESCAPE , Quit
    >;

  constexpr int resolution = 1080;

  Game::initGame ();
  Game g
    { "Breakout"       // Window title
    , resolution*16/9  // Window width
    , resolution       // Window height
    , kbs{}            // Input manager
    , true             // Lazy redraw
    };

  g.loop <ZipperS <MainMenu, Breakout, Highscore>> ();
}
