#include "Game.h"
#include "InputManager.h"
#include "Breakout.h"
#include "MainMenu.h"
#include "DualScene.h"


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

  initGame ();

  std::cout << sizeof(std::tuple<MainMenu, Breakout>) << std::endl;
  std::cout << sizeof(NScene<MainMenu, Breakout>) << std::endl;

  Game<NScene<MainMenu, Breakout>> g
    { "Breakout", 1000, 1000, im };

  g.loop();
}
