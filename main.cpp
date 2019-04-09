#include <iostream>
#include "Game.h"
#include "InputManager.h"
#include "Breakout.h"
#include "MainMenu.h"
#include "NScene.h"
#include "Highscore.h"


int main ([[maybe_unused]] int argc, [[maybe_unused]] const char *argv[])
{
  InputManager im = InputManager
    ( SDLK_h      , MoveLeft
    , SDLK_l      , MoveRight
    , SDLK_j      , MoveDown
    , SDLK_k      , MoveUp

    , SDLK_SPACE  , ReleaseBall
    , SDLK_f      , FireRocket
    , SDLK_d      , PowerMagnet

    , SDLK_ESCAPE , Quit
    );

  constexpr int resolution = 1080;

  Game::initGame ();
  Game g {"Breakout", resolution*16/9, resolution, im};
  g.loop <NScene<Highscore, MainMenu, Breakout>> ();
}
